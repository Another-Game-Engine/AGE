#include <BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h>

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
				getData()->data = new btRigidBody(static_cast<btScalar>(1.0f), new btDefaultMotionState(), nullptr);
				assert(getData()->data != nullptr && "Impossible to create actor");
				btRigidBody *body = getDataAs<btRigidBody>();
				body->setCollisionShape(&BulletRigidBody::EmptyShape);
				body->setFlags(body->getFlags() | BT_DISABLE_WORLD_GRAVITY);
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				body->setActivationState(DISABLE_DEACTIVATION);
				static_cast<BulletWorld *>(world)->getWorld()->addRigidBody(body);
			}
			else
			{
				rigidBody = static_cast<BulletRigidBody *>(getDataAs<btRigidBody>()->getUserPointer());
			}
			btRigidBody *body = getDataAs<btRigidBody>();
			body->setUserPointer(this);
			body->setCollisionShape(collisionShape);
			setAsTrigger(IsTriggerByDefault());
			setFilterGroup(GetDefaultFilterGroup());
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

		void BulletCollider::updateShape(btCollisionShape *shape)
		{
			assert(shape != nullptr && "Invalid shape");
			btRigidBody *body = getDataAs<btRigidBody>();
			body->setCollisionShape(shape);
			collisionShape = shape;
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

		void BulletCollider::setMaterial(const std::string &name)
		{
			MaterialInterface *newMaterial = getWorld()->createMaterial(name);
			static_cast<ColliderInterface *>(this)->setMaterial(newMaterial);
		}

		void BulletCollider::setPosition(const glm::vec3 &position)
		{
			btTransform &transform = getDataAs<btRigidBody>()->getWorldTransform();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
		}

		void BulletCollider::setRotation(const glm::quat &rotation)
		{
			btTransform &transform = getDataAs<btRigidBody>()->getWorldTransform();
			transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
		}
	}
}