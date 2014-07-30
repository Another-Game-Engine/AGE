#pragma once

#include <Utils/DependenciesInjector.hpp>
#include <memory>
#include <Components/ComponentRegistrar.hpp>
#include <Core/EntityIdRegistrar.hh>

#include <list>
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
#include <Utils/Containers/Queue.hpp>

class System;
class Engine;
class EntityFilter;

class AScene : public DependenciesInjector, public ComponentRegistrar, public EntityIdRegistrar
{
private:
	std::multimap<std::size_t, std::shared_ptr<System> >                    _systems;
	std::array<std::list<EntityFilter*>, MAX_CPT_NUMBER + MAX_TAG_NUMBER>   _filters;
	std::array<AComponentManager*, MAX_CPT_NUMBER>                          _componentsManagers;
	std::array<EntityData, MAX_ENTITY_NUMBER>                               _pool;
	AGE::Queue<std::uint16_t>                                               _free;
	ENTITY_ID                                                               _entityNumber;
	int test = 0;
public:
	AScene(std::weak_ptr<Engine> &&engine);
	virtual ~AScene();
	inline std::uint16_t    getNumberOfEntities() { return _entityNumber - static_cast<ENTITY_ID>(_free.size()); }
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

	void                    informFiltersTagAddition(TAG_ID id, EntityData &&entity);
	void                    informFiltersTagDeletion(TAG_ID id, EntityData &&entity);
	void                    informFiltersComponentAddition(COMPONENT_ID id, EntityData &&entity);
	void                    informFiltersComponentDeletion(COMPONENT_ID id, EntityData &&entity);

	Entity &createEntity();
	void destroy(const Entity &e);

	//const glm::mat4 &getTransform(const Entity &e) const;
	//glm::mat4 &getTransformRef(const Entity &e);
	//void setTransform(Entity &e, const glm::mat4 &trans);

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
		informFiltersTagAddition(tag, std::move(data));
	}

	void removeTag(Entity &e, TAG_ID tag)
	{
		auto &data = _pool[e.id];
		if (data.entity != e)
			return;
		data.barcode.unsetTag(tag);
		informFiltersTagDeletion(tag, std::move(data));
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

		informFiltersComponentAddition(COMPONENT_ID(T::getTypeId()), std::move(e));
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

	Component::Base *getComponent(const Entity &entity, COMPONENT_ID componentId)
	{
		auto &e = _pool[entity.id];
		assert(e.entity == entity);
		assert(e.barcode.hasComponent(componentId));
		return this->_componentsManagers[componentId]->getComponentPtr(entity);
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
		informFiltersComponentDeletion(id, std::move(e));
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
