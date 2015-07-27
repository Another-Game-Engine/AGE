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
		void NullRigidBody::setAngularDrag(float angularDrag)
		{
			this->angularDrag = angularDrag;
		}

		float NullRigidBody::getAngularDrag(void) const
		{
			return angularDrag;
		}

		void NullRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			this->angularVelocity = angularVelocity;
		}

		glm::vec3 NullRigidBody::getAngularVelocity(void) const
		{
			return angularVelocity;
		}

		void NullRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			this->centerOfMass = centerOfMass;
		}

		glm::vec3 NullRigidBody::getCenterOfMass(void) const
		{
			return centerOfMass;
		}

		void NullRigidBody::setLinearDrag(float linearDrag)
		{
			this->linearDrag = linearDrag;
		}

		float NullRigidBody::getLinearDrag(void) const
		{
			return linearDrag;
		}

		void NullRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			this->linearVelocity = linearVelocity;
		}

		glm::vec3 NullRigidBody::getLinearVelocity(void) const
		{
			return linearVelocity;
		}

		void NullRigidBody::setMass(float mass)
		{
			this->mass = mass;
		}

		float NullRigidBody::getMass(void) const
		{
			return mass;
		}

		void NullRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
		{
			this->diagonalInertiaTensor = diagonalInertiaTensor;
		}

		glm::vec3 NullRigidBody::getDiagonalInertiaTensor(void) const
		{
			return diagonalInertiaTensor;
		}

		void NullRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			this->maxAngularVelocity = maxAngularVelocity;
		}

		float NullRigidBody::getMaxAngularVelocity(void) const
		{
			return maxAngularVelocity;
		}

		void NullRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			this->maxDepenetrationVelocity = maxDepenetrationVelocity;
		}

		float NullRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return maxDepenetrationVelocity;
		}

		void NullRigidBody::setPosition(const glm::vec3 &position)
		{
			this->position = position;
		}

		glm::vec3 NullRigidBody::getPosition(void) const
		{
			return position;
		}

		void NullRigidBody::setRotation(const glm::quat &rotation)
		{
			this->rotation = rotation;
		}

		glm::quat NullRigidBody::getRotation(void) const
		{
			return rotation;
		}

		void NullRigidBody::affectByGravity(bool mustBeAffectedByGravity)
		{
			affectedByGravity = mustBeAffectedByGravity;
		}

		bool NullRigidBody::isAffectedByGravity(void) const
		{
			return affectedByGravity;
		}

		void NullRigidBody::setAsKinematic(bool mustBeKinematic)
		{
			kinematic = mustBeKinematic;
		}

		bool NullRigidBody::isKinematic(void) const
		{
			return kinematic;
		}

		void NullRigidBody::setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode)
		{
			this->collisionDetectionMode = collisionDetectionMode;
		}

		CollisionDetectionMode NullRigidBody::getCollisionDetectionMode(void) const
		{
			return collisionDetectionMode;
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

		void NullRigidBody::addAbsoluteTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			return;
		}

		void NullRigidBody::addRelativeTorque(const glm::vec3 &torque, ForceMode forceMode)
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