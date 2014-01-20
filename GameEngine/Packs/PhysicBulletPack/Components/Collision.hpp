#ifndef   __COLLISION_HPP__
# define  __COLLISION_HPP__

#include <Components/Component.hh>

namespace Component
{
	class Collision : public ComponentBase<Collision>
	{
	public:
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
			_collsions.clear();
		}

		void addCollision(const Entity &entity)
		{
			_collsions.insert(entity);
		}

		std::set<Entity> &getCollisions()
		{
			return _collsions;
		}

		void clear()
		{
			_collsions.clear();
		}
	private:
		std::set<Entity> _collsions;
	};
}

#endif    //__COLLISION_HPP__