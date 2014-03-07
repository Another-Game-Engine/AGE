#ifndef  __COLLISION_CLEANER_SYSTEM_HPP__
# define __COLLISION_CLEANER_SYSTEM_HPP__

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

class CollisionCleaner : public System
{
public:
	CollisionCleaner(AScene *scene)
		: System(scene)
		, _filter(scene)
	{
		_name = "collision_cleaner_system";
	}
	virtual ~CollisionCleaner(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto collection = _filter.getCollection();
		for (auto &it = std::begin(collection); it != std::end(collection); )
		{
			auto e = *it;
			++it;
			e->removeComponent<Component::Collision>();
		}
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::Collision>();
	}
};

#endif   //__COLLISION_CLEANER_SYSTEM_HPP__