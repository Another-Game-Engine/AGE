#pragma once

#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Core/EntityIdRegistrationManager.hh>
#include <list>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/map.hpp>
#include <Utils/Containers/Queue.hpp>
#include <Entities/EntitySerializationInfos.hpp>
#include <Utils/ObjectPool.hpp>
#include <unordered_set>
#include <Components/ComponentManager.hpp>
#include <Utils/Debug.hpp>
#include <fstream>
#include "SceneChunkSerialization.hpp"


namespace AGE
{
	class Engine;
	class RenderScene;
	class EntityFilter;
	class System;
	class SceneManager;
#ifdef AGE_BFC
	class BFCLinkTracker;
	class BFCBlockManagerFactory;
#endif

	class AScene : public DependenciesInjector, public EntityIdRegistrationManager, public ComponentManager
	{
	private:
		std::multimap<std::size_t, std::shared_ptr<System> >                    _systems;
		std::array<std::list<EntityFilter*>, MAX_CPT_NUMBER + MAX_TAG_NUMBER>   _filters;
		std::list<EntityFilter*>                                                 _allFilters;
		AGE::ObjectPool<EntityData>                                             _entityPool;
		AGE::Queue<std::uint16_t>                                               _freeEntityId;
		std::unordered_set<Entity>                                              _entities;
		ENTITY_ID                                                               _entityNumber;
		AGE::RenderScene                                                        *_renderScene;
		bool                                                                    _active;
#ifdef AGE_BFC
		BFCLinkTracker                                                          *_bfcLinkTracker;
		BFCBlockManagerFactory                                                  *_bfcBlockManagerFactory;
#endif
		friend EntityFilter;
		friend class AGE::RenderScene;
		friend class AGE::SceneManager;
	protected:
		AGE::Engine *                                              _engine;
		inline void setActive(bool tof) { _active = tof; }
	public:
#ifdef AGE_BFC
		BFCLinkTracker *getBfcLinkTracker() { return _bfcLinkTracker; }
		BFCBlockManagerFactory *getBfcBlockManagerFactory() { return _bfcBlockManagerFactory; }
#endif
		AScene(AGE::Engine *engine);
		virtual ~AScene();
		inline std::size_t      getNumberOfEntities() const { return _entities.size(); }
		bool                    userStart();
		bool                    userUpdateBegin(float time);
		bool                    userUpdateEnd(float time);
		virtual bool 			_userStart() = 0;
		virtual bool 			_userUpdateBegin(float time) = 0;
		virtual bool            _userUpdateEnd(float time) = 0;
		void 					update(float time);
		bool                    start();
		inline AGE::Engine *getEngine() { return _engine; }
		inline void setRenderScene(AGE::RenderScene *renderScene) { _renderScene = renderScene; }
		inline bool isActive() const { return _active; }
		inline RenderScene *getRenderScene() { return _renderScene; }

		void                    registerFilter(EntityFilter *filter);
		void                    filterSubscribe(ComponentType id, EntityFilter* filter);
		void                    filterUnsubscribe(ComponentType id, EntityFilter* filter);

		void                    informFiltersTagAddition(TAG_ID id, const EntityData &entity);
		void                    informFiltersTagDeletion(TAG_ID id, const EntityData &entity);
		void                    informFiltersComponentAddition(ComponentType id, const EntityData &entity);
		void                    informFiltersComponentDeletion(ComponentType id, const EntityData &entity);
		void                    informFiltersEntityCreation(const EntityData &entity);
		void                    informFiltersEntityDeletion(const EntityData &entity);

		// If `outContext` is true, entity will not be in the world and updated
		// It's used for Archetypes, a priori, you don't need it anywhere else.
		Entity &createEntity(bool outContext = false);

		// deep will destroy all children recursively
		// if not deep children will be set as root level
		void destroy(const Entity &e, bool deep = false);

		bool copyEntity(const Entity &source, Entity &destination, bool deep = true, bool outContext = false);

		void clearAllEntities();

		template <typename T, typename... Args>
		std::shared_ptr<T> addSystem(std::size_t priority, Args &&...args)
		{
			SCOPE_profile_cpu_function("Scenes");
			auto tmp = std::make_shared<T>((AScene*)(this), std::forward<Args>(args)...);
			if (!tmp->init())
				return nullptr;
			_systems.insert(std::make_pair(priority, tmp));
			return tmp;
		}

		template <typename T>
		std::shared_ptr<T> getSystem()
		{
			SCOPE_profile_cpu_function("Scenes");
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
			SCOPE_profile_cpu_function("Scenes");
			for (auto &e : _systems)
			{
				if (typeid(*e.second.get()).name() == typeid(T).name())
				{
					e.second->finalize();
					_systems.erase(std::find(_systems.begin(), _systems.end(), e));
					return;
				}
			}
		}

		template <typename T>
		bool activateSystem()
		{
			SCOPE_profile_cpu_function("Scenes");
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

		void save(const std::string &fileName);
		void load(const std::string &fileName);

		////////////////////////
		///////
		// Component Manager Get / Set

		template <typename T>
		void clearComponentsType()
		{
			SCOPE_profile_cpu_function("Scenes");
			//TODO
			auto id = Component<T>::getTypeId();
			if (_componentsManagers[id] == nullptr)
				return;
			auto &manager = *static_cast<ComponentManager<T>*>(_componentsManagers[id]);
			auto &col = manager.getComponents();
			for (std::size_t i = 0; i < manager.getSize(); ++i)
			{
				_entityPool[col[i].entityId].barcode.unsetComponent(ComponentType(id));
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
	};
}
