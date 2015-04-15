#include <Systems/CollisionSystem.hpp>
#include <Physics/Collision.hpp>

namespace AGE
{
	namespace Private
	{
		CollisionSystem::CollisionSystem(AScene *scene)
			: System(scene), entityFilter(scene)
		{
			_name = "CollisionSystem";
			entityFilter.requireComponent<Collider>();
		}

		bool CollisionSystem::initialize(void)
		{
			_scene->getInstance<Physics::WorldInterface>()->setCollisionListener(this);
			return true;
		}

		void CollisionSystem::finalize(void)
		{
			_scene->getInstance<Physics::WorldInterface>()->setCollisionListener(nullptr);
		}

		void CollisionSystem::mainUpdate(float elapsedTime)
		{
			for (Entity entity : entityFilter.getCollection())
			{
				entity.getComponent<Collider>()->collisions.clear();
			}
		}

		void CollisionSystem::onCollision(Collider *currentCollider, Collider *hitCollider, std::vector<Physics::Contact> contacts, Physics::ContactType contactType)
		{
			currentCollider->collisions.push_back(Physics::Collision(hitCollider, contacts, contactType));
		}
	}
}