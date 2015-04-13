#include <Systems/CollisionSystem.hpp>

namespace AGE
{
	namespace Private
	{
		CollisionSystem::CollisionSystem(AScene *scene, Physics::PhysicsInterface *physics)
			: System(scene), physics(physics), entityFilter(scene)
		{
			_name = "CollisionSystem";
			// TO_DO
			// entityFilter.requireComponent<Collider>();
		}

		bool CollisionSystem::initialize(void)
		{
			// TO_DO
			return true;
		}

		void CollisionSystem::finalize(void)
		{
			// TO_DO
		}

		void CollisionSystem::mainUpdate(float elapsedTime)
		{
			// TO_DO
		}
	}
}