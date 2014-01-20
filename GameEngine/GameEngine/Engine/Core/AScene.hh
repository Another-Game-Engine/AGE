#ifndef ASCENE_HH_
#define ASCENE_HH_

#include <Entities/EntityData.hh>
//#include <Systems/System.h>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/SmartPointer.hh>
#include <list>
#include <queue>
#include <map>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

class Engine;
class System;


class AScene : public DependenciesInjector
{
private:
	std::multimap<std::size_t, SmartPointer<System> >   _systems;
	std::vector<EntityData>                             _pool;
	std::queue<unsigned int>                            _free;
protected:
	Engine                                              &_engine;
public:
	AScene(Engine &engine);
	virtual ~AScene();

	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);
	Entity &createEntity();
	void destroy(const Entity &h);
	EntityData *get(const Entity &h);

	Engine &getEngine()
	{
		return _engine;
	}

	template <typename T>
	SmartPointer<T> addSystem(std::size_t priority)
	{
		SmartPointer<System> tmp = new T(this);
		_systems.insert(std::make_pair(priority, tmp));
		tmp->init();
		return tmp;
	}

	template <typename T>
	SmartPointer<T> getSystem()
	{
		for (auto &e : _systems)
		{
			if (typeid(*e.second.get()).name() == typeid(T).name())
				return e.second;
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
	AScene &rct()
	{
		std::cout << "Multi : " << typeid(T).name() << std::endl;
		return *this;
	}

	template <typename Archive>
	void save(std::ofstream &s)
	{}

};

#endif