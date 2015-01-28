#pragma once

#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Components/ComponentRegistrar.hpp>
#include <Core/EntityIdRegistrar.hh>
#include <list>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/map.hpp>
#include <Core/ComponentManager.hpp>
#include <Utils/Containers/Queue.hpp>
#include <Entities/EntitySerializationInfos.hpp>

namespace AGE
{
	class Engine;
	class RenderScene;
	class EntityFilter;
	class System;
	class SceneManager;

	class AScene : public DependenciesInjector, public EntityIdRegistrar
	{
	private:
		std::multimap<std::size_t, std::shared_ptr<System> >                    _systems;
		std::array<std::list<EntityFilter*>, MAX_CPT_NUMBER + MAX_TAG_NUMBER>   _filters;
		std::list<EntityFilter*>                                                 _allFilters;
		std::array<AComponentManager*, MAX_CPT_NUMBER>                          _componentsManagers;
		std::array<EntityData, MAX_ENTITY_NUMBER>                               _pool;
		AGE::Queue<std::uint16_t>                                               _free;
		ENTITY_ID                                                               _entityNumber;
		AGE::RenderScene                                                        *_renderScene;
		bool                                                                    _active;
		friend EntityFilter;
		friend class AGE::RenderScene;
		friend class AGE::SceneManager;
	protected:
		std::weak_ptr<AGE::Engine>                                              _engine;
		inline void setActive(bool tof) { _active = tof; }
	public:
		AScene(std::weak_ptr<AGE::Engine> engine);
		virtual ~AScene();
		inline std::uint16_t    getNumberOfEntities() const { return _entityNumber - static_cast<ENTITY_ID>(_free.size()); }
		virtual bool 			userStart() = 0;
		virtual bool 			userUpdateBegin(double time) = 0;
		virtual bool            userUpdateEnd(double time) = 0;
		void 					update(double time);
		bool                    start();
		inline std::weak_ptr<AGE::Engine> getEngine() { return _engine; }
		inline void setRenderScene(AGE::RenderScene *renderScene) { _renderScene = renderScene; }
		inline bool isActive() const { return _active; }

		void                    registerFilter(EntityFilter *filter);
		void                    filterSubscribe(COMPONENT_ID id, EntityFilter* filter);
		void                    filterUnsubscribe(COMPONENT_ID id, EntityFilter* filter);

		void                    informFiltersTagAddition(TAG_ID id, const EntityData &entity);
		void                    informFiltersTagDeletion(TAG_ID id, const EntityData &entity);
		void                    informFiltersComponentAddition(COMPONENT_ID id, const EntityData &entity);
		void                    informFiltersComponentDeletion(COMPONENT_ID id, const EntityData &entity);
		void                    informFiltersEntityCreation(const EntityData &entity);
		void                    informFiltersEntityDeletion(const EntityData &entity);

		Entity &createEntity();
		void destroy(const Entity &e);
		void clearAllEntities();

		template <typename T>
		std::shared_ptr<T> addSystem(std::size_t priority)
		{
			auto tmp = std::make_shared<T>(std::static_pointer_cast<AScene>(shared_from_this()));
			if (!tmp->init())
				return nullptr;
			_systems.insert(std::make_pair(priority, tmp));
			return tmp;
		}

		template <typename T>
		std::shared_ptr<T> getSystem()
		{
			for (auto &e : _systems)
			{
				if (typeid(*e.second.get()).name() == typeid(T).name())
					return std::static_pointer_cast<T>(e.second);
			}
			return nullptr;
		}

		template <typename T>
		void deleteSystem()
		{
			for (auto &e : _systems)
			{
				if (typeid(*e.second.get()).name() == typeid(T).name())
				{
					delete e.second;
					_systems.erase(e);
					return;
				}
			}
		}


		template <typename T>
		bool activateSystem()
		{
			for (auto &e : _systems)
			{
				if (typeid(*e.second.get()).name() == typeid(T).name())
					return e.second->setActivation(true);
			}
			return false;
		}

		template <typename T>
		bool deactivateSystem()
		{
			for (auto &e : _systems)
			{
				if (typeid(*e.second.get()).name() == typeid(T).name())
					return e.second->setActivation(false);
			}
			return false;
		}

		void saveToJson(const std::string &fileName);
		void loadFromJson(const std::string &fileName);
		void saveToBinary(const std::string &fileName);
		void loadFromBinary(const std::string &fileName);


		template <typename Archive>
		void save(std::ofstream &s)
		{
			Archive ar(s);

			//// we save type database
			//ar(cereal::make_nvp("Types_database", _typeDatabase));

			std::uint16_t entityNbr = getNumberOfEntities();

			ar(cereal::make_nvp("Number_of_serialized_entities", entityNbr));

			std::vector<EntityData> entities;

			// we list entities
			auto ctr = 0;
			for (auto &e : _pool)
			{
				if (e.entity.isActive())
				{
					entities.push_back(e);
					++ctr;
					if (ctr >= entityNbr)
						break;
				}
			}

			for (auto &e : entities)
			{
				auto es = EntitySerializationInfos(e);
				for (COMPONENT_ID i = 0; i < MAX_CPT_NUMBER; ++i)
				{
					if (e.barcode.hasComponent(i))
					{
						auto cpt = getComponent(e.getEntity(), i);
						auto hash_code = getComponentHash(i);
						es.componentsHash.push_back(hash_code);
						es.components.push_back(getComponent(e.getEntity(), i));
					}
				}
				ar(cereal::make_nvp("Entity_" + std::to_string(e.getEntity().getId()), es));
				es.serializeComponents(ar, this);
			}
		}

		template <typename Archive>
		void load(std::ifstream &s)
		{
			Archive ar(s);

			std::uint16_t size = 0;
			ar(size);
			for (unsigned int i = 0; i < size; ++i)
			{
				auto &e = createEntity();
				auto &ed = _pool[e.getId()];

				EntitySerializationInfos infos(ed);
				ar(infos);
				e.flags = infos.flags;
				ed.link.setPosition(infos.link.getPosition());
				ed.link.setOrientation(infos.link.getOrientation());
				ed.link.setScale(infos.link.getScale());

				for (auto &hash : infos.componentsHash)
				{
					std::size_t componentTypeId;
					auto ptr = ComponentRegistrar::getInstance().createComponentFromType(hash, ar, componentTypeId, this);
					ed.barcode.setComponent(componentTypeId);
					assert(_componentsManagers[componentTypeId] != nullptr);
					_componentsManagers[componentTypeId]->addComponentPtr(e, ptr);
					informFiltersComponentAddition(componentTypeId, ed);
				}
				//	ar(*e.get());
			}
			//updateEntityHandles();
		}


		////////////////////////
		///////
		// Component Manager Get / Set

		template <typename T>
		void registerComponentType()
		{
			COMPONENT_ID id = COMPONENT_ID(T::getTypeId());
			if (_componentsManagers[id] == nullptr)
			{
				_componentsManagers[id] = new ComponentManager<T>(this);
			}
			REGISTER_COMPONENT_TYPE(T);
		}

		template <typename T>
		void clearComponentsType()
		{
			//TODO
			auto id = T::getTypeId();
			if (_componentsManagers[id] == nullptr)
				return;
			auto &manager = *static_cast<ComponentManager<T>*>(_componentsManagers[id]);
			auto &col = manager.getComponents();
			for (std::size_t i = 0; i < manager.getSize(); ++i)
			{
				_pool[col[i].entityId].barcode.unsetComponent(COMPONENT_ID(id));
			}
			manager.clearComponents();
			for (auto filter : _filters[id])
			{
				filter->clearCollection();
			}
		}

		void addTag(Entity &e, TAG_ID tag);
		void removeTag(Entity &e, TAG_ID tag);
		bool isTagged(Entity &e, TAG_ID tag);

		////////////////////////
		//////
		// Component operation

		// Components operations with handle
		template <typename T, typename... Args>
		T *addComponent(Entity &entity, Args &&...args)
		{
			COMPONENT_ID id = COMPONENT_ID(T::getTypeId());
			auto &e = _pool[entity.id];
			if (e.entity != entity)
				return nullptr;
			if (_componentsManagers[id] == nullptr)
			{
				_componentsManagers[id] = new ComponentManager<T>(this);
			}
			if (e.barcode.hasComponent(id))
			{
				return static_cast<ComponentManager<T>*>(_componentsManagers[T::getTypeId()])->getComponent(entity);
			}

			auto res = static_cast<ComponentManager<T>*>(_componentsManagers[id])->addComponent(entity, std::forward<Args>(args)...);

			e.barcode.setComponent(id);

			informFiltersComponentAddition(COMPONENT_ID(T::getTypeId()), e);
			return res;
		}

		template <typename T>
		T *getComponent(const Entity &entity)
		{
			COMPONENT_ID id = COMPONENT_ID(T::getTypeId());
			auto &e = _pool[entity.id];
			assert(e.entity == entity);
			//return nullptr;
			assert(e.barcode.hasComponent(id));
			//return nullptr;
			return static_cast<ComponentManager<T>*>(_componentsManagers[id])->getComponent(entity);
		}

		template <typename T>
		bool *hasComponent(const Entity &entity)
		{
			COMPONENT_ID id = COMPONENT_ID(T::getTypeId());
			auto &e = _pool[entity.id];
			assert(e.entity == entity);
			return (e.barcode.hasComponent(id));
		}

		Component::Base *getComponent(const Entity &entity, COMPONENT_ID componentId);
		bool hasComponent(const Entity &entity, COMPONENT_ID componentId);
		std::size_t getComponentHash(COMPONENT_ID componentId);

		template <typename T>
		bool removeComponent(Entity &entity)
		{
			COMPONENT_ID id = T::getTypeId();
			auto &e = _pool[entity.id];
			if (e.entity != entity)
				return false;
			if (!e.barcode.hasComponent(id))
			{
				return false;
			}
			static_cast<ComponentManager<T>*>(_componentsManagers[id])->removeComponent(entity);
			e.barcode.unsetComponent(id);
			informFiltersComponentDeletion(id, e);
			return true;
		}

		void reorganizeComponents();
		const Entity *getEntityPtr(const Entity &e) const;
		const Entity &getEntityFromId(ENTITY_ID id) const;
		AComponentManager *getComponentManager(COMPONENT_ID componentId);
		AGE::Link *getLink(const Entity &e);
		AGE::Link *getLink(const ENTITY_ID &id);
	};
}
