#include <cassert>

#include <Physics/ICollisionListener.hpp>
#include <Systems/CollisionSystem.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		ICollisionListener::ICollisionListener(AScene *scene)
			: scene(scene)
		{
			assert(scene != nullptr && "Invalid scene");
			std::shared_ptr<::AGE::Private::CollisionSystem> collisionSystem = scene->getSystem<::AGE::Private::CollisionSystem>();
			assert(collisionSystem != nullptr && "CollisionSystem not found");
			collisionSystem->addListener(this);
		}

		// Destructors
		ICollisionListener::~ICollisionListener(void)
		{
			std::shared_ptr<::AGE::Private::CollisionSystem> collisionSystem = scene->getSystem<::AGE::Private::CollisionSystem>();
			assert(collisionSystem != nullptr && "CollisionSystem not found");
			collisionSystem->removeListener(this);
		}

		// Virtual Methods
		void ICollisionListener::onCollisionEnter(Entity currentEntity, Entity hitEntity, const std::vector<Physics::Contact> &contacts)
		{
			return;
		}

		void ICollisionListener::onCollisionStay(Entity currentEntity, Entity hitEntity, const std::vector<Physics::Contact> &contacts)
		{
			return;
		}

		void ICollisionListener::onCollisionExit(Entity currentEntity, Entity hitEntity, const std::vector<Physics::Contact> &contacts)
		{
			return;
		}
	}
}