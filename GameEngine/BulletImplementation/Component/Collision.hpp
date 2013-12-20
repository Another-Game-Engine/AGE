#ifndef   __COLLISION_HPP__
# define  __COLLISION_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"

namespace Component
{
	class Collision : public ComponentBase<Collision>
	{
	public:
		Collision(Engine &engine, Handle &entity)
			: ComponentBase<Collision>(engine, entity, "CollisionComponent")
		{
				
		}

		virtual ~Collision()
		{
		}

		void addCollision(const Handle &entity)
		{
			_collsions.insert(entity);
		}

		std::set<Handle> &getCollisions()
		{
			return _collsions;
		}

		void clear()
		{
			_collsions.clear();
		}
	private:
		std::set<Handle> _collsions;
	};
}

#endif    //__COLLISION_HPP__