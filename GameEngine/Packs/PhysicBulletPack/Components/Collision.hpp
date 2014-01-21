#ifndef   __COLLISION_HPP__
# define  __COLLISION_HPP__

#include <Components/Component.hh>

namespace Component
{
	struct Collision : public ComponentBase<Collision>
	{
		Collision()
			: ComponentBase<Collision>()
		{
				
		}

		virtual ~Collision()
		{
		}

		void init()
		{}

		virtual void reset()
		{
			collisions.clear();
		}

		void addCollision(const Entity &entity)
		{
			collisions.insert(entity);
		}

		std::set<Entity> &getCollisions()
		{
			return collisions;
		}

		void clear()
		{
			collisions.clear();
		}

		std::set<Entity> collisions;
	};
}

#endif    //__COLLISION_HPP__