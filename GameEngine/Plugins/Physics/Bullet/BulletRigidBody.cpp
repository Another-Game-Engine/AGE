#include "BulletRigidBody.hpp"
#include "BulletCollider.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Attributes
		btEmptyShape BulletRigidBody::EmptyShape;

		// Constructors
		BulletRigidBody::BulletRigidBody(BulletWorld *world, Private::GenericData *data)
			: RigidBodyInterface(world, data)
		{
			if (getData()->data == nullptr)
			{
				getData()->data = new btRigidBody(static_cast<btScalar>(0.0f), new btDefaultMotionState(), nullptr);
				assert(getData()->data != nullptr && "Impossible to create actor");
				btRigidBody *body = getDataAs<btRigidBody>();
				body->setCollisionShape(&BulletRigidBody::EmptyShape);
				world->getWorld()->addRigidBody(body);
				body->setUserPointer(this);
			}
			else
			{
				btRigidBody *body = getDataAs<btRigidBody>();
				static_cast<BulletCollider *>(body->getUserPointer())->rigidBody = this;
				const glm::vec3 worldGravity = static_cast<WorldInterface *>(world)->getGravity();
				body->setGravity(btVector3(worldGravity.x, worldGravity.y, worldGravity.z));
			}
			setAngularDrag(GetDefaultAngularDrag());
			setAngularVelocity(GetDefaultAngularVelocity());
			setCenterOfMass(GetDefaultCenterOfMass());
			setLinearDrag(GetDefaultLinearDrag());
			setLinearVelocity(GetDefaultLinearVelocity());
			setMass(GetDefaultMass());
			setDiagonalInertiaTensor(GetDefaultDiagonalInertiaTensor());
			setMaxAngularVelocity(GetDefaultMaxAngularVelocity());
			setMaxDepenetrationVelocity(GetDefaultMaxDepenetrationVelocity());
			affectByGravity(IsAffectedByGravityByDefault());
			setAsKinematic(IsKinematicByDefault());
			setCollisionDetectionMode(GetDefaultCollisionDetectionMode());
		}

		// Destructor
		BulletRigidBody::~BulletRigidBody(void)
		{
			btRigidBody *body = getDataAs<btRigidBody>();
			BulletCollider *collider = static_cast<BulletCollider *>(body->getUserPointer());
			if (collider == nullptr || static_cast<void *>(collider) == this)
			{
				static_cast<BulletWorld *>(getWorld())->getWorld()->removeRigidBody(body);
				delete body;
				getData()->data = nullptr;
			}
			else
			{
				affectByGravity(false);
				setAsKinematic(true);
				setMass(0.0f);
				collider->rigidBody = nullptr;
			}
		}

		// Inherited Methods
		void BulletRigidBody::setAngularDrag(float angularDrag)
		{
			getDataAs<btRigidBody>()->setAngularFactor(1.0f - angularDrag);
		}

		float BulletRigidBody::getAngularDrag(void) const
		{
			return 1.0f - getDataAs<btRigidBody>()->getAngularDamping();
		}

		void BulletRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			getDataAs<btRigidBody>()->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
		}

		glm::vec3 BulletRigidBody::getAngularVelocity(void) const
		{
			const btVector3 &angularVelocity = getDataAs<btRigidBody>()->getAngularVelocity();
			return glm::vec3(angularVelocity.x(), angularVelocity.y(), angularVelocity.z());
		}

		void BulletRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(centerOfMass.x, centerOfMass.y, centerOfMass.z));
			getDataAs<btRigidBody>()->setCenterOfMassTransform(transform);
		}

		glm::vec3 BulletRigidBody::getCenterOfMass(void) const
		{
			const btVector3 &centerOfMass = getDataAs<btRigidBody>()->getCenterOfMassPosition();
			return glm::vec3(centerOfMass.x(), centerOfMass.y(), centerOfMass.z());
		}

		void BulletRigidBody::setLinearDrag(float linearDrag)
		{
			const float drag = 1.0f - linearDrag;
			getDataAs<btRigidBody>()->setLinearFactor(btVector3(drag, drag, drag));
		}

		float BulletRigidBody::getLinearDrag(void) const
		{
			return 1.0f - getDataAs<btRigidBody>()->getLinearDamping();
		}

		void BulletRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			getDataAs<btRigidBody>()->setLinearVelocity(btVector3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
		}

		glm::vec3 BulletRigidBody::getLinearVelocity(void) const
		{
			const btVector3 &linearVelocity = getDataAs<btRigidBody>()->getLinearVelocity();
			return glm::vec3(linearVelocity.x(), linearVelocity.y(), linearVelocity.z());
		}

		void BulletRigidBody::setMass(float mass)
		{
			const glm::vec3 diagonalInertiaTensor = getDiagonalInertiaTensor();
			getDataAs<btRigidBody>()->setMassProps(mass, btVector3(diagonalInertiaTensor.x, diagonalInertiaTensor.y, diagonalInertiaTensor.z));
		}

		float BulletRigidBody::getMass(void) const
		{
			const float invMass = getDataAs<btRigidBody>()->getInvMass();
			return invMass == 0.0f ? std::numeric_limits<float>::max() : 1.0f / invMass;
		}

		void BulletRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
		{
			const float mass = getMass();
			getDataAs<btRigidBody>()->setMassProps(mass, btVector3(diagonalInertiaTensor.x, diagonalInertiaTensor.y, diagonalInertiaTensor.z));
		}

		glm::vec3 BulletRigidBody::getDiagonalInertiaTensor(void) const
		{
			const btVector3 &invDiagonalInertiaTensor = getDataAs<btRigidBody>()->getInvInertiaDiagLocal();
			const float x = invDiagonalInertiaTensor.x() == 0.0f ? std::numeric_limits<float>::max() : 1.0f / invDiagonalInertiaTensor.x();
			const float y = invDiagonalInertiaTensor.y() == 0.0f ? std::numeric_limits<float>::max() : 1.0f / invDiagonalInertiaTensor.y();
			const float z = invDiagonalInertiaTensor.z() == 0.0f ? std::numeric_limits<float>::max() : 1.0f / invDiagonalInertiaTensor.z();
			return glm::vec3(x, y, z);
		}

		void BulletRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			this->maxAngularVelocity = maxAngularVelocity;
		}

		float BulletRigidBody::getMaxAngularVelocity(void) const
		{
			return maxAngularVelocity;
		}

		void BulletRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			this->maxDepenetrationVelocity = maxDepenetrationVelocity;
		}

		float BulletRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return maxDepenetrationVelocity;
		}

		void BulletRigidBody::setPosition(const glm::vec3 &position)
		{
			btTransform &transform = getDataAs<btRigidBody>()->getWorldTransform();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
		}

		glm::vec3 BulletRigidBody::getPosition(void) const
		{
			const btVector3 &position = getDataAs<btRigidBody>()->getWorldTransform().getOrigin();
			return glm::vec3(position.x(), position.y(), position.z());
		}

		void BulletRigidBody::setRotation(const glm::quat &rotation)
		{
			btTransform &transform = getDataAs<btRigidBody>()->getWorldTransform();
			transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
		}

		glm::quat BulletRigidBody::getRotation(void) const
		{
			const btQuaternion rotation = getDataAs<btRigidBody>()->getWorldTransform().getRotation();
			return glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
		}

		void BulletRigidBody::affectByGravity(bool mustBeAffectedByGravity)
		{
			btRigidBody *body = getDataAs<btRigidBody>();
			body->setFlags(mustBeAffectedByGravity ? body->getFlags() & ~BT_DISABLE_WORLD_GRAVITY : body->getFlags() | BT_DISABLE_WORLD_GRAVITY);
		}

		bool BulletRigidBody::isAffectedByGravity(void) const
		{
			btRigidBody const *body = getDataAs<btRigidBody>();
			return !(body->getFlags() & BT_DISABLE_WORLD_GRAVITY);
		}

		void BulletRigidBody::setAsKinematic(bool mustBeKinematic)
		{
			btRigidBody *body = getDataAs<btRigidBody>();
			if (mustBeKinematic)
			{
				body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				body->setActivationState(DISABLE_DEACTIVATION);
			}
			else
			{
				body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
				body->setActivationState(ACTIVE_TAG);
			}
		}

		bool BulletRigidBody::isKinematic(void) const
		{
			const btRigidBody *body = getDataAs<btRigidBody>();
			return (body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) && body->getActivationState() == DISABLE_DEACTIVATION;
		}

		void BulletRigidBody::setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode)
		{
			return;
		}

		CollisionDetectionMode BulletRigidBody::getCollisionDetectionMode(void) const
		{
			return CollisionDetectionMode::Continuous;
		}

		void BulletRigidBody::addForce(const glm::vec3 &force, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case AGE::Physics::ForceMode::Force:
					getDataAs<btRigidBody>()->applyCentralForce(btVector3(force.x, force.y, force.z));
					break;
				case AGE::Physics::ForceMode::Acceleration:
					getDataAs<btRigidBody>()->applyCentralForce(btVector3(force.x, force.y, force.z));
					break;
				case AGE::Physics::ForceMode::Impulse:
					getDataAs<btRigidBody>()->applyCentralImpulse(btVector3(force.x, force.y, force.z));
					break;
				case AGE::Physics::ForceMode::VelocityChange:
					getDataAs<btRigidBody>()->applyCentralForce(btVector3(force.x, force.y, force.z));
					break;
				default:
					break;
			}
		}

		void BulletRigidBody::addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			btRigidBody *body = getDataAs<btRigidBody>();
			const btVector3 worldPosition = body->getWorldTransform().inverse() * btVector3(position.x, position.y, position.z);
			switch (forceMode)
			{
				case AGE::Physics::ForceMode::Force:
					body->applyForce(btVector3(force.x, force.y, force.z), btVector3(worldPosition.x(), worldPosition.y(), worldPosition.z()));
					break;
				case AGE::Physics::ForceMode::Acceleration:
					body->applyForce(btVector3(force.x, force.y, force.z), btVector3(worldPosition.x(), worldPosition.y(), worldPosition.z()));
					break;
				case AGE::Physics::ForceMode::Impulse:
					body->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(worldPosition.x(), worldPosition.y(), worldPosition.z()));
					break;
				case AGE::Physics::ForceMode::VelocityChange:
					body->applyForce(btVector3(force.x, force.y, force.z), btVector3(worldPosition.x(), worldPosition.y(), worldPosition.z()));
					break;
				default:
					break;
			}
		}

		void BulletRigidBody::addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case AGE::Physics::ForceMode::Force:
					getDataAs<btRigidBody>()->applyForce(btVector3(force.x, force.y, force.z), btVector3(position.x, position.y, position.z));
					break;
				case AGE::Physics::ForceMode::Acceleration:
					getDataAs<btRigidBody>()->applyForce(btVector3(force.x, force.y, force.z), btVector3(position.x, position.y, position.z));
					break;
				case AGE::Physics::ForceMode::Impulse:
					getDataAs<btRigidBody>()->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(position.x, position.y, position.z));
					break;
				case AGE::Physics::ForceMode::VelocityChange:
					getDataAs<btRigidBody>()->applyForce(btVector3(force.x, force.y, force.z), btVector3(position.x, position.y, position.z));
					break;
				default:
					break;
			}
		}

		void BulletRigidBody::addAbsoluteTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case AGE::Physics::ForceMode::Force:
					getDataAs<btRigidBody>()->applyTorque(btVector3(torque.x, torque.y, torque.z));
					break;
				case AGE::Physics::ForceMode::Acceleration:
					getDataAs<btRigidBody>()->applyTorque(btVector3(torque.x, torque.y, torque.z));
					break;
				case AGE::Physics::ForceMode::Impulse:
					getDataAs<btRigidBody>()->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
					break;
				case AGE::Physics::ForceMode::VelocityChange:
					getDataAs<btRigidBody>()->applyTorque(btVector3(torque.x, torque.y, torque.z));
					break;
				default:
					break;
			}
		}

		void BulletRigidBody::addRelativeTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			const btVector3 absoluteTorque = getDataAs<btRigidBody>()->getWorldTransform() * btVector3(torque.x, torque.y, torque.z);
			addAbsoluteTorque(glm::vec3(absoluteTorque.x(), absoluteTorque.y(), absoluteTorque.z()), forceMode);
		}

		glm::vec3 BulletRigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
		{
			const btVector3 localPosition = getDataAs<btRigidBody>()->getWorldTransform().inverse() * btVector3(position.x, position.y, position.z);
			return getVelocityAtLocalPosition(glm::vec3(localPosition.x(), localPosition.y(), localPosition.z()));
		}

		glm::vec3 BulletRigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
		{
			const btVector3 velocity = getDataAs<btRigidBody>()->getVelocityInLocalPoint(btVector3(position.x, position.y, position.z));
			return glm::vec3(velocity.x(), velocity.y(), velocity.z());
		}
	}
}