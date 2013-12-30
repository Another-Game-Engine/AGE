#ifndef ASCENE_HH_
#define ASCENE_HH_

#include "Entities/Entity.hh"
#include <Systems/System.h>
#include <Utils/DependenciesInjector.hpp>

#include <list>
#include <queue>
#include <map>

class AScene : public DependenciesInjector
{
private:
	std::list<SmartPointer<Entity> >                    _collection;
	std::queue<SmartPointer<Entity> >                   _pool;
	std::multimap<std::size_t, SmartPointer<System> >   _systems;
protected:
	Engine                                              &_engine;
public:
	AScene(Engine &engine);
	virtual ~AScene();

	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate(double time) = 0;
	void 					update(double time);

	template <typename T>
	SmartPointer<T> addSystem(std::size_t priority)
	{
		SmartPointer<System> tmp = new T(_engine);
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
};

#endif