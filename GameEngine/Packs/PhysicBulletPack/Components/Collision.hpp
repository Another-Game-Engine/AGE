#ifndef   __COLLISION_HPP__
# define  __COLLISION_HPP__

#include <Components/Component.hh>

namespace Component
{
	class Collision : public ComponentBase<Collision>
	{
	public:
		Collision(AScene *scene, Handle &entity)
			: ComponentBase<Collision>(scene, entity, "CollisionComponent")
		{
				
		}

		virtual ~Collision()
		{
		}

		void init()
		{}

		virtual void reset()
		{
			_collsions.clear();
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