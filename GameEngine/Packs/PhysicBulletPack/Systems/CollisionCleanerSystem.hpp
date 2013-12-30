#ifndef  __COLLISION_CLEANER_SYSTEM_HPP__
# define __COLLISION_CLEANER_SYSTEM_HPP__

#include <Systems/System.h>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

class CollisionCleaner : public System
{
public:
	CollisionCleaner(AScene *scene) : System(scene)
	{}
	virtual ~CollisionCleaner(){}
private:
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto &it = std::begin(_collection); it != std::end(_collection); )
		{
			auto e = *it;
			++it;
			e->removeComponent<Component::Collision>();
		}
	}

	virtual void initialize()
	{
		require<Component::Collision>();
	}
};

#endif   //__COLLISION_CLEANER_SYSTEM_HPP__