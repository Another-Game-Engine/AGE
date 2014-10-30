#pragma once

#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Components/ComponentRegistrar.hpp>
#include <Core/EntityIdRegistrar.hh>

#include <list>
#include <map>
#include <array>

#include <glm/fwd.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/map.hpp>
#include <Components/ComponentRegistrar.hpp>

#include <Core/ComponentManager.hpp>
#include <Utils/Containers/Queue.hpp>
#include <Entities/EntitySerializationInfos.hpp>

//je remplace l'age queue par une std::queue pour le test
#include <queue>

class System;
class Engine;
class EntityFilter;

class AScene : public DependenciesInjector, public EntityIdRegistrar
{
private:
	std::multimap<std::size_t, std::shared_ptr<System> >                    _systems;
	std::array<std::list<EntityFilter*>, MAX_CPT_NUMBER + MAX_TAG_NUMBER>   _filters;
	std::array<AComponentManager*, MAX_CPT_NUMBER>                          _componentsManagers;
	std::array<EntityData, MAX_ENTITY_NUMBER>                               _pool;
	std::queue<std::uint16_t>                                               _free;
	ENTITY_ID                                                               _entityNumber;
public:
	AScene(std::weak_ptr<Engine> &&engine);
	virtual ~AScene();
	inline std::uint16_t    getNumberOfEntities() const { return _entityNumber - static_cast<ENTITY_ID>(_free.size()); }
	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);
	bool                    start();

	void                    filterSubscribe(COMPONENT_ID id, EntityFilter* filter)
	{
		auto findIter = std::find(_filters[id].begin(), _filters[id].end(), filter);
		if (findIter == std::end(_filters[id]))
			_filters[id].push_back(filter);
	}

	void                    filterUnsubscribe(COMPONENT_ID id, EntityFilter* filter)
	{
		_filters[id].remove(filter);
	}

	void                    informFiltersTagAddition(TAG_ID id, const EntityData &entity);
	void                    informFiltersTagDeletion(TAG_ID id, const EntityData &entity);
	void                    informFiltersComponentAddition(COMPONENT_ID id, const EntityData &entity);
	void                    informFiltersComponentDeletion(COMPONENT_ID id, const EntityData &entity);

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


	template <typename Archive>
	void save(std::ofstream &s)
	{
		Archive ar(s);

		//// we save type database
		//ar(cereal::make_nvp("Types_database", _typeDatabase));

		auto entityNbr = getNumberOfEntities();

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

		unsigned int size = 0;
		ar(size);
		for (unsigned int i = 0; i < size; ++i)
		{
			auto &e = createEntity();
			auto &ed = _pool[e.getId()];

			EntitySerializationInfos infos(ed);
			ar(infos);
			e.flags = infos.flags;
			ed.link = infos.link;

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


	void addTag(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return;
		data.barcode.setTag(tag);
		informFiltersTagAddition(tag, data);
	}

	void removeTag(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return;
		data.barcode.unsetTag(tag);
		informFiltersTagDeletion(tag, data);
	}

	bool isTagged(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return false;
		return data.barcode.hasTag(tag);
	}

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

	Component::Base *getComponent(const Entity &entity, COMPONENT_ID componentId)
	{
		auto &e = _pool[entity.id];
		assert(e.entity == entity);
		assert(e.barcode.hasComponent(componentId));
		return this->_componentsManagers[componentId]->getComponentPtr(entity);
	}

	bool hasComponent(const Entity &entity, COMPONENT_ID componentId)
	{
		auto &e = _pool[entity.id];
		assert(e.entity == entity);
		return (e.barcode.hasComponent(componentId));
	}

	std::size_t getComponentHash(COMPONENT_ID componentId)
	{
		assert(this->_componentsManagers[componentId] != nullptr);
		return this->_componentsManagers[componentId]->getHashCode();
	}

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

	void reorganizeComponents()
	{
		for (auto &&e : _componentsManagers)
		{
			if (e != nullptr)
				e->reorder();
		}
	}

	const Entity *getEntityPtr(const Entity &e) const
	{
		auto &entity = _pool[e.id];
		if (entity.entity != e)
			return nullptr;
		return &(entity.entity);
	}

	const Entity &getEntityFromId(ENTITY_ID id) const
	{
		return _pool[id].entity;
	}

	AComponentManager *getComponentManager(COMPONENT_ID componentId)
	{
		return _componentsManagers[componentId];
	}

	AGE::Link *getLink(const Entity &e)
	{
		return &_pool[e.id].link;
	}

	AGE::Link *getLink(const ENTITY_ID &id)
	{
		return &_pool[id].link;
	}

private:
	friend EntityFilter;
};
