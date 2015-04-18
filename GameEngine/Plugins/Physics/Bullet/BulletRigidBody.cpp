#include "BulletRigidBody.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletRigidBody::BulletRigidBody(BulletWorld *world, Private::GenericData *data)
			: RigidBodyInterface(world, data)
		{
			// TO_DO
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

		// Inherited Methods
		void BulletRigidBody::setAngularDrag(float angularDrag)
		{
			// TO_DO
		}

		float BulletRigidBody::getAngularDrag(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getAngularVelocity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getCenterOfMass(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setLinearDrag(float linearDrag)
		{
			// TO_DO
		}

		float BulletRigidBody::getLinearDrag(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getLinearVelocity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setMass(float mass)
		{
			// TO_DO
		}

		float BulletRigidBody::getMass(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getDiagonalInertiaTensor(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			// TO_DO
		}

		float BulletRigidBody::getMaxAngularVelocity(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			// TO_DO
		}

		float BulletRigidBody::getMaxDepenetrationVelocity(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setPosition(const glm::vec3 &position)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getPosition(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setRotation(const glm::quat &rotation)
		{
			// TO_DO
		}

		glm::quat BulletRigidBody::getRotation(void) const
		{
			// TO_DO
			return glm::quat();
		}

		void BulletRigidBody::affectByGravity(bool mustBeAffectedByGravity)
		{
			// TO_DO
		}

		bool BulletRigidBody::isAffectedByGravity(void) const
		{
			// TO_DO
			return false;
		}

		void BulletRigidBody::setAsKinematic(bool mustBeKinematic)
		{
			// TO_DO
		}

		bool BulletRigidBody::isKinematic(void) const
		{
			// TO_DO
			return false;
		}

		void BulletRigidBody::setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode)
		{
			// TO_DO
		}

		CollisionDetectionMode BulletRigidBody::getCollisionDetectionMode(void) const
		{
			// TO_DO
			return CollisionDetectionMode::Discrete;
		}

		void BulletRigidBody::addForce(const glm::vec3 &force, ForceMode forceMode)
		{
			// TO_DO
		}

		void BulletRigidBody::addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			// TO_DO
		}

		void BulletRigidBody::addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			// TO_DO
		}

		void BulletRigidBody::addAbsoluteTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			// TO_DO
		}

		void BulletRigidBody::addRelativeTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
		{
			// TO_DO
			return glm::vec3();
		}

		glm::vec3 BulletRigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
		{
			// TO_DO
			return glm::vec3();
		}
	}
}