#include <SystemsCore/CollisionSystem.hpp>
#include <Physics/Collision.hpp>
#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	namespace Private
	{
		// Constructors
		CollisionSystem::CollisionSystem(AScene *scene)
			: System(scene), entityFilter(scene)
		{
			_name = "CollisionSystem";
			entityFilter.requireComponent<Collider>();
		}

		// Methods
		void CollisionSystem::addListener(Physics::ICollisionListener *listener)
		{
			listeners.insert(listener);
		}

		void CollisionSystem::removeListener(Physics::ICollisionListener *listener)
		{
			listeners.erase(listener);
		}

		// Inherited Methods
		bool CollisionSystem::initialize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->setCollisionListener(this);
			return true;
		}

		void CollisionSystem::finalize(void)
		{
			_scene->getInstance<Physics::PhysicsInterface>()->getWorld()->setCollisionListener(nullptr);
		}

		void CollisionSystem::mainUpdate(float elapsedTime)
		{
			for (const Entity &entity : entityFilter.getCollection())
			{
				entity->getComponent<Collider>()->collisions.clear();
			}
		}

		void CollisionSystem::onCollision(Collider *currentCollider, Collider *hitCollider, const std::vector<Physics::Contact> &contacts, Physics::CollisionType CollisionType)
		{
			currentCollider->collisions.push_back(Physics::Collision(hitCollider, contacts, CollisionType));
			switch (CollisionType)
			{
				case Physics::CollisionType::New:
				{
					for (Physics::ICollisionListener *listener : listeners)
					{
						listener->onCollisionEnter(currentCollider->entity, hitCollider->entity, contacts);
					}
					break;
				}
				case Physics::CollisionType::Persistent:
				{
					for (Physics::ICollisionListener *listener : listeners)
					{
						listener->onCollisionStay(currentCollider->entity, hitCollider->entity, contacts);
					}
					break;
				}
				case Physics::CollisionType::Lost:
				{
					for (Physics::ICollisionListener *listener : listeners)
					{
						listener->onCollisionExit(currentCollider->entity, hitCollider->entity, contacts);
					}
					break;
				}
				default:
				{
					assert(!"Never reached");
					break;
				}
			}
		}
	}
}