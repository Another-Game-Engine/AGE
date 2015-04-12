#include "PhysXRigidBody.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXRigidBody::PhysXRigidBody(PhysXWorld *world, const glm::vec3 &position)
			: RigidBodyInterface(static_cast<WorldInterface *>(world)), actor(static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createRigidDynamic(physx::PxTransform(position.x, position.y, position.z)))
		{
			assert(actor != nullptr && "Impossible to create actor");
			world->getScene()->addActor(*actor);
		}

		// Inherited Methods
		void PhysXRigidBody::setAngularDrag(float angularDrag)
		{
			actor->setAngularDamping(angularDrag);
		}

		float PhysXRigidBody::getAngularDrag(void) const
		{
			return actor->getAngularDamping();
		}

		void PhysXRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			actor->setAngularVelocity(physx::PxVec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
		}

		glm::vec3 PhysXRigidBody::getAngularVelocity(void) const
		{
			const physx::PxVec3 angularVelocity = actor->getAngularVelocity();
			return glm::vec3(angularVelocity.x, angularVelocity.y, angularVelocity.z);
		}

		void PhysXRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			actor->setCMassLocalPose(physx::PxTransform(centerOfMass.x, centerOfMass.y, centerOfMass.z));
		}

		glm::vec3 PhysXRigidBody::getCenterOfMass(void) const
		{
			const physx::PxVec3 centerOfMass = actor->getCMassLocalPose().p;
			return glm::vec3(centerOfMass.x, centerOfMass.y, centerOfMass.z);
		}

		void PhysXRigidBody::setLinearDrag(float linearDrag)
		{
			actor->setLinearDamping(linearDrag);
		}

		float PhysXRigidBody::getLinearDrag(void) const
		{
			return actor->getLinearDamping();
		}

		void PhysXRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			actor->setLinearVelocity(physx::PxVec3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
		}

		glm::vec3 PhysXRigidBody::getLinearVelocity(void) const
		{
			const physx::PxVec3 linearVelocity = actor->getLinearVelocity();
			return glm::vec3(linearVelocity.x, linearVelocity.y, linearVelocity.z);
		}

		void PhysXRigidBody::setMass(float mass)
		{
			actor->setMass(mass);
		}

		float PhysXRigidBody::getMass(void) const
		{
			return actor->getMass();
		}

		void PhysXRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			actor->setMaxAngularVelocity(maxAngularVelocity);
		}

		float PhysXRigidBody::getMaxAngularVelocity(void) const
		{
			return actor->getMaxAngularVelocity();
		}

		void PhysXRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			actor->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
		}

		float PhysXRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return actor->getMaxDepenetrationVelocity();
		}

		void PhysXRigidBody::setPosition(const glm::vec3 &position)
		{
			physx::PxTransform transform = actor->getGlobalPose();
			transform.p.x = position.x;
			transform.p.y = position.y;
			transform.p.z = position.z;
			actor->setGlobalPose(transform);
		}

		glm::vec3 PhysXRigidBody::getPosition(void) const
		{
			const physx::PxVec3 position = actor->getGlobalPose().p;
			return glm::vec3(position.x, position.y, position.z);
		}

		void PhysXRigidBody::setRotation(const glm::quat &rotation)
		{
			physx::PxTransform transform = actor->getGlobalPose();
			transform.q.x = rotation.x;
			transform.q.y = rotation.y;
			transform.q.z = rotation.z;
			transform.q.w = rotation.w;
			actor->setGlobalPose(transform);
		}

		glm::quat PhysXRigidBody::getRotation(void) const
		{
			const physx::PxQuat rotation = actor->getGlobalPose().q;
			return glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		}
	}
}