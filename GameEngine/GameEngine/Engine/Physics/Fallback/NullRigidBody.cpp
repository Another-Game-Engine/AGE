#include <Physics/Fallback/NullRigidBody.hpp>
#include <Physics/Fallback/NullWorld.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullRigidBody::NullRigidBody(NullWorld *world, Private::GenericData *data)
			: RigidBodyInterface(world, data)
		{
			return;
		}

		// Inherited Methods
		void NullRigidBody::setAngularDrag(float angularDrag)
		{
			return;
		}

		float NullRigidBody::getAngularDrag(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			return;
		}

		glm::vec3 NullRigidBody::getAngularVelocity(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			return;
		}

		glm::vec3 NullRigidBody::getCenterOfMass(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setLinearDrag(float linearDrag)
		{
			return;
		}

		float NullRigidBody::getLinearDrag(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			return;
		}

		glm::vec3 NullRigidBody::getLinearVelocity(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setMass(float mass)
		{
			return;
		}

		float NullRigidBody::getMass(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
		{
			return;
		}

		glm::vec3 NullRigidBody::getDiagonalInertiaTensor(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			return;
		}

		float NullRigidBody::getMaxAngularVelocity(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			return;
		}

		float NullRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setPosition(const glm::vec3 &position)
		{
			bodyPosition = position;
		}

		glm::vec3 NullRigidBody::getPosition(void) const
		{
			return bodyPosition;
		}

		void NullRigidBody::setRotation(const glm::quat &rotation)
		{
			bodyRotation = rotation;
		}

		glm::quat NullRigidBody::getRotation(void) const
		{
			return bodyRotation;
		}

		void NullRigidBody::affectByGravity(bool mustBeAffectedByGravity)
		{
			return;
		}

		bool NullRigidBody::isAffectedByGravity(void) const
		{
			return false;
		}

		void NullRigidBody::setAsKinematic(bool mustBeKinematic)
		{
			return;
		}

		bool NullRigidBody::isKinematic(void) const
		{
			return false;
		}

		void NullRigidBody::setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode)
		{
			return;
		}

		CollisionDetectionMode NullRigidBody::getCollisionDetectionMode(void) const
		{
			return CollisionDetectionMode::Discrete;
		}

		void NullRigidBody::addForce(const glm::vec3 &force, ForceMode forceMode)
		{
			return;
		}

		void NullRigidBody::addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			return;
		}

		void NullRigidBody::addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			return;
		}

		void NullRigidBody::addTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			return;
		}

		glm::vec3 NullRigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
		{
			return glm::vec3();
		}

		glm::vec3 NullRigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
		{
			return glm::vec3();
		}
	}
}