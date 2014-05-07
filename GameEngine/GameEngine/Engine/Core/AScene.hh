#pragma once

#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Components/ComponentRegistrar.hpp>
#include <Core/EntityIdRegistrar.hh>

#include <list>
#include <queue>
#include <map>
#include <array>

#include <glm/glm.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <Components/ComponentRegistrar.hpp>

#include <Core/ComponentManager.hpp>

class System;
class Engine;
class EntityFilter;

class AScene : public DependenciesInjector, public ComponentRegistrar, public EntityIdRegistrar
{
private:
	std::multimap<std::size_t, std::shared_ptr<System> >                    _systems;
	std::array<std::list<EntityFilter*>, MAX_CPT_NUMBER + MAX_TAG_NUMBER>   _filters;
	std::array<AComponentManager*, MAX_CPT_NUMBER>                          _componentsManagers;
	std::array<Entity, UINT16_MAX>                                          _pool;
	std::array<std::uint16_t, UINT16_MAX>                                   _infosIndices;
	std::vector<std::array<std::uint16_t, MAX_CPT_NUMBER>>                  _componentsRefs;
	std::vector<glm::mat4>                                                  _localTransform;
	std::vector<glm::mat4>                                                  _globalTransform;
	std::vector<std::array<std::uint16_t, 255>>                             _graphnode;
	std::queue<std::uint16_t>                                               _free;
	ENTITY_ID                                                               _entityNumber;
public:
	AScene(std::weak_ptr<Engine> &&engine);
	virtual ~AScene();
	inline std::uint16_t    getNumberOfEntities() { return _entityNumber - static_cast<ENTITY_ID>(_free.size()); }
	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);
	bool                    start();
	void                    filterSubscribe(unsigned short, EntityFilter* filter);
	void                    filterUnsubscribe(unsigned short, EntityFilter* filter);
	void                    informFilters(bool added, std::uint8_t id, Entity &&entity);

	Entity &createEntity();
	void destroy(Entity &h);

	glm::mat4 &getLocalTransform(const Entity &e);
	glm::mat4 &getGlobalTransform(const Entity &e);

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

	template <typename Archive>
	void save(std::ofstream &s)
	{
		Archive ar(s);
		unsigned int size = 0;
		for (auto &e : _pool)
		{
			// TODO
			//if (e.getFlags() & EntityData::ACTIVE)
			//{
			//	++size;
			//}
		}
		ar(cereal::make_nvp("Number_of_serialized_entities", size));
		for (auto &e : _pool)
		{
			// TODO
			//if (e.getFlags() & EntityData::ACTIVE)
			//{
			//	ar(cereal::make_nvp("Entity_" + std::to_string(e.getHandle().getId()), e));
			//}
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
			auto e = createEntity();
			ar(*e.get());
		}
		updateEntityHandles();
	}


	////////////////////////
	///////
	// Component Manager Get / Set
	template <typename T>
	ComponentManager<T> &getComponentManager()
	{
		auto id = T::getTypeId();

		if (_componentsManagers.size() <= id)
		{
			_componentsManagers.resize(id + 1, nullptr);
			_componentsManagers[id] = new ComponentManager<T>(this);
		}
		return *static_cast<ComponentManager<T>*>(_componentsManagers[id]);
	}

	template <typename T>
	void clearComponentsType()
	{
		auto &manager = getComponentManager<T>();
		auto &componentTable = manager.getComponentRefs();
		auto s = componentTable.size();
		auto id = T::getTypeId();

		for (std::size_t i = 0; i < s; ++i)
		{
			auto position = componentTable[i];
			_pool[position].unsetComponent(id);
		}
		manager.clearComponents();
		for (auto filter : _filters[id + MAX_TAG_NUMBER])
		{
			filter->clearCollection();
		}
	}

	////////////////////////
	//////
	// Component operation

	// Components operations with handle
	template <typename T, typename... Args>
	T *addComponent(Entity &entity, Args &&...args)
	{
		std::size_t id = T::getTypeId();

		if (!_componentsManagers[id])
			_componentsManagers[id] = new ComponentManager<T>(this);
		auto res = static_cast<ComponentManager<T>*>(_componentsManagers[id])->addComponent(entity, std::forward<Args>(args)...);
		informFilters(true, T::getTypeId() + MAX_TAG_NUMBER, std::move(entity));
		return res;
	}

	template <typename T>
	T *getComponent(const Entity &entity)
	{
		auto index = _componentsRefs[T::getTypeId()][_infosIndices[entity.id]];
		return static_cast<ComponentManager<T>*>(_componentsManagers[T::getTypeId()])->getComponent(entity, index);
	}

	template <typename T>
	bool removeComponent(Entity &entity)
	{
		auto id = T::getTypeId();
		auto index = _componentsRefs[id][_infosIndices[entity.id]];
		auto res = static_cast<ComponentManager<T>*>(_componentsManagers[T::getTypeId()])->removeComponent(entity, index);
		entity.unsetComponent(id);
		informFilters(false, id + MAX_TAG_NUMBER, std::move(entity));
		return res;
	}

	void reorganizeComponents()
	{
		for (auto &&e : _componentsManagers)
		{
			if (e != nullptr)
				e->reorder();
		}
	}

};
