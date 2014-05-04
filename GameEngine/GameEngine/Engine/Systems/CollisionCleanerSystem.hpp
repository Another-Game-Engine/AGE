#ifndef  __COLLISION_CLEANER_SYSTEM_HPP__
# define __COLLISION_CLEANER_SYSTEM_HPP__

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

class CollisionCleaner : public System
{
public:
	CollisionCleaner(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
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
		auto scene = _scene.lock();
		EntityFilter::Lock lock(_filter);
		for (auto &&e : _filter.getCollection())
		{
			scene->removeComponent<Component::Collision>(e.getId());
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::Collision>();
		return true;
	}
};

#endif   //__COLLISION_CLEANER_SYSTEM_HPP__