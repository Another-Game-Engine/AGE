#include "BulletCollider.hpp"
#include "BulletWorld.hpp"
#include "BulletRigidBody.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletCollider::BulletCollider(WorldInterface *world, Private::GenericData *data, btCollisionShape *collisionShape)
			: ColliderInterface(world, data), collisionShape(collisionShape)
		{
			assert(collisionShape != nullptr && "Invalid collision shape");
			if (getData()->data == nullptr)
			{
				getData()->data = new btRigidBody(static_cast<btScalar>(0.0f), new btDefaultMotionState(), nullptr);
				assert(getData()->data != nullptr && "Impossible to create actor");
				btRigidBody *body = getDataAs<btRigidBody>();
				body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				body->setActivationState(DISABLE_DEACTIVATION);
				body->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));
			}
			else
			{
				btRigidBody *body = getDataAs<btRigidBody>();
				static_cast<BulletWorld *>(world)->getWorld()->removeRigidBody(body);
				rigidBody = static_cast<BulletRigidBody *>(body->getUserPointer());
			}
			btRigidBody *body = getDataAs<btRigidBody>();
			body->setUserPointer(this);
			body->setCollisionShape(collisionShape);
			static_cast<BulletWorld *>(world)->getWorld()->addRigidBody(body, static_cast<short>(filterGroup), static_cast<short>(-1));
			setAsTrigger(IsTriggerByDefault());
		}

		// Destructor
		BulletCollider::~BulletCollider(void)
		{
			btRigidBody *body = getDataAs<btRigidBody>();
			btDiscreteDynamicsWorld *bulletWorld = static_cast<BulletWorld *>(getWorld())->getWorld();
			bulletWorld->removeRigidBody(body);
			if (rigidBody == nullptr)
			{
				delete body;
				getData()->data = nullptr;
			}
			else
			{
				body->setUserPointer(nullptr);
				body->setCollisionShape(&BulletRigidBody::EmptyShape);
				bulletWorld->addRigidBody(body);
			}
			delete collisionShape;
			collisionShape = nullptr;
		}

		// Methods
		btCollisionShape *BulletCollider::getShape(void)
		{
			return collisionShape;
		}

		const btCollisionShape *BulletCollider::getShape(void) const
		{
			return collisionShape;
		}

		// Inherited Methods
		void BulletCollider::setAsTrigger(bool mustBeATrigger)
		{
			btRigidBody *body = getDataAs<btRigidBody>();
			body->setCollisionFlags(mustBeATrigger ? body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE : body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		bool BulletCollider::isATrigger(void) const
		{
			return (getDataAs<btRigidBody>()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) != 0;
		}

		void BulletCollider::setFilterGroup(FilterGroup group)
		{
			filterGroup = group;
			btRigidBody *body = getDataAs<btRigidBody>();
			btDiscreteDynamicsWorld *bulletWorld = static_cast<BulletWorld *>(getWorld())->getWorld();
			bulletWorld->removeRigidBody(body);
			bulletWorld->addRigidBody(body, static_cast<short>(filterGroup), static_cast<short>(-1));
		}

		FilterGroup BulletCollider::getFilterGroup(void) const
		{
			return filterGroup;
		}

		void BulletCollider::scale(const glm::vec3 &scaling)
		{
			collisionShape->setLocalScaling(collisionShape->getLocalScaling() * btVector3(scaling.x, scaling.y, scaling.z));
		}
	}
}