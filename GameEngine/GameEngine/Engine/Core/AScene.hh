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

class AScene : public DependenciesInjector, public ComponentRegistrar, public EntityIdRegistrar
{
private:
	std::multimap<std::size_t, std::shared_ptr<System> >   _systems;
	std::vector<EntityData>                             _pool;
	std::queue<unsigned int>                            _free;
	std::size_t                                         _entityNumber;
public:
	AScene(std::weak_ptr<Engine> engine);
	virtual ~AScene();
	inline std::size_t getNumberOfEntities() { return _entityNumber; }
	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);
	bool                    start();
	Entity &createEntity();
	void destroy(const Entity &h);
	EntityData *get(const Entity &h);

	template <typename T>
	std::shared_ptr<T> addSystem(std::size_t priority)
	{
		auto tmp = std::make_shared<T>(std::static_pointer_cast<AScene>(shared_from_this()));
		_systems.insert(std::make_pair(priority, tmp));
		tmp->init();
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
		std::map<unsigned int, unsigned int> unserializedId;

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

};
