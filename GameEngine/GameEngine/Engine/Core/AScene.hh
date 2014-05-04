#pragma once

#include <Entities/EntityData.hh>
//#include <Systems/System.h>
#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Components/ComponentRegistrar.hpp>
#include <Core/EntityIdRegistrar.hh>

#include <list>
#include <queue>
#include <map>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <Components/ComponentRegistrar.hpp>

class System;
class Engine;
class EntityFilter;

class AScene : public DependenciesInjector, public ComponentRegistrar, public EntityIdRegistrar
{
private:
	std::multimap<std::size_t, std::shared_ptr<System> >_systems;
	std::vector<EntityData>                             _pool;
	std::queue<std::size_t>                             _free;
	std::size_t                                         _entityNumber;
	std::map<unsigned short, std::list<EntityFilter*>> _filters;
public:
	AScene(std::weak_ptr<Engine> &&engine);
	virtual ~AScene();
	inline std::size_t getNumberOfEntities() { return _entityNumber; }
	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);
	bool                    start();
	void filterSubscribe(unsigned short, EntityFilter* filter);
	void filterUnsubscribe(unsigned short, EntityFilter* filter);
	void informFilters(bool added, unsigned short id, Entity &&entity);

	Entity &createEntity();
	void destroy(const Entity &h);
	EntityData *get(const Entity &h);

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
			if (e.getFlags() & EntityData::ACTIVE)
			{
				++size;
			}
		}
		ar(cereal::make_nvp("Number_of_serialized_entities", size));
		for (auto &e : _pool)
		{
			if (e.getFlags() & EntityData::ACTIVE)
			{
				ar(cereal::make_nvp("Entity_" + std::to_string(e.getHandle().getId()), e));
			}
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
	//////
	// Component operation


	std::vector<void*> _components;
	std::vector<std::vector<std::uint32_t /*index in _components table*/>> _componentsRefs;
	std::vector<std::vector<std::pair<std::size_t /*entityId*/, std::size_t /*index in components refs*/>>> _componentsEntity;

	// Components operations with id
	template <typename T, typename... Args>
	inline T *addComponent(std::size_t entity, Args &&...args)
	{
		return addComponent<T>(_pool[entity], args...);
	}

	template <typename T>
	inline T *getComponent(std::size_t entity)
	{
		return getComponent<T>(_pool[entity]);
	}

	template <typename T>
	inline bool hasCompoment(std::size_t entity)
	{
		return hasComponent<T>(_pool[entity].hasComponent<T>());
	}

	template <typename T>
	inline bool removeComponent(std::size_t entity)
	{
		return removeComponent<T>(_pool[entity]);
	}

	// Components operations with handle
	template <typename T, typename... Args>
	T *addComponent(Entity &entity, Args &&...args)
	{
		return addComponent<T>(*this->get(entity), args...);
	}

	template <typename T>
	T *getComponent(Entity &entity)
	{
		return getComponent<T>(*this->get(entity));
	}

	template <typename T>
	bool hasCompoment(Entity &entity)
	{
		return (this->get(entity)->hasComponent<T>());
	}

	template <typename T>
	bool removeComponent(Entity &entity)
	{
		return removeComponent<T>(this->get(entity));
	}

	// Components operations with entitydata
	template <typename T, typename... Args>
	T *addComponent(EntityData &entity, Args &&...args)
	{
		// get the component type ID
		std::size_t id = T::getTypeId();

		// if entity already have component, return it
		if (entity.hasComponent<T>())
		{
			// TODO -> Get component
			return getComponent<T>(entity);
		}
		// else if entity components array is to small, resize it
		else if (_components.size() <= id)
		{
			_components.resize(id + 1);
			_componentsRefs.resize(id + 1);
			_componentsEntity.resize(id + 1);
			_components[id] = new std::vector<T>();
		}

		std::vector<T> *TComponentList = static_cast<std::vector<T>*>(_components[id]);

		auto position = TComponentList->size();
		auto refPosition = _componentsRefs[id].size();
		_componentsRefs[id].push_back(position);
		if (_componentsEntity[id].size() <= position)
			_componentsEntity[id].resize(position + 1);
		_componentsEntity[id][position] = std::make_pair(entity.getHandle().getId(), refPosition);
		TComponentList->emplace_back(T());

		auto &cptable = entity.componentsTable;
		if (entity.componentsTable.size() <= id)
			entity.componentsTable.resize(id + 1);// , (std::size_t)(-1));
		entity.componentsTable[id] = refPosition;
		entity.getCode().add(id + MAX_TAG_NUMBER);

		//init component
		T *component = static_cast<T*>(&(TComponentList->back()));
		component->init(std::forward<Args>(args)...);
		informFilters(true, id + MAX_TAG_NUMBER, std::move(entity.getHandle()));
		return component;
	}

	template <typename T>
	T *getComponent(EntityData &e) const
	{
		// get the component type ID
		unsigned short id = T::getTypeId();

		if (!e.hasComponent<T>() || _components.size() <= id)
			return nullptr;

		std::vector<T> *TComponentList = static_cast<std::vector<T>*>(_components[id]);
		return &(*TComponentList)[_componentsRefs[id][e.componentsTable[id]]];
	}

	template <typename T>
	bool removeComponent(EntityData &e)
	{
		// get the component type ID
		unsigned short id = T::getTypeId();

		if (!e.hasComponent<T>() || _components.size() <= id)
			return false;
		(static_cast<std::vector<T>*>(_components[id]))->at(_componentsRefs[id][e.componentsTable[id]]).reset();
		e.componentsTable[id] = (std::size_t)(-1);

		e.getCode().remove(id + MAX_TAG_NUMBER);
		informFilters(false, id + MAX_TAG_NUMBER, std::move(e.getHandle()));

		return true;
	}

};
