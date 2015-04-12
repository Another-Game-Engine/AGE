#include "PhysXRigidBody.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXRigidBody::PhysXRigidBody(PhysXWorld *world, const glm::vec3 &position)
			: RigidBodyInterface(static_cast<WorldInterface *>(world)), body(static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createRigidDynamic(physx::PxTransform(position.x, position.y, position.z)))
		{
			assert(body != nullptr && "Impossible to create body");
			world->getScene()->addActor(*body);
		}

		// Inherited Methods
		void PhysXRigidBody::setAngularDrag(float angularDrag)
		{
			body->setAngularDamping(angularDrag);
		}

		float PhysXRigidBody::getAngularDrag(void) const
		{
			return body->getAngularDamping();
		}

		void PhysXRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			body->setAngularVelocity(physx::PxVec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
		}

		glm::vec3 PhysXRigidBody::getAngularVelocity(void) const
		{
			const physx::PxVec3 angularVelocity = body->getAngularVelocity();
			return glm::vec3(angularVelocity.x, angularVelocity.y, angularVelocity.z);
		}

		void PhysXRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			body->setCMassLocalPose(physx::PxTransform(centerOfMass.x, centerOfMass.y, centerOfMass.z));
		}

		glm::vec3 PhysXRigidBody::getCenterOfMass(void) const
		{
			const physx::PxVec3 centerOfMass = body->getCMassLocalPose().p;
			return glm::vec3(centerOfMass.x, centerOfMass.y, centerOfMass.z);
		}

		void PhysXRigidBody::setLinearDrag(float linearDrag)
		{
			body->setLinearDamping(linearDrag);
		}

		float PhysXRigidBody::getLinearDrag(void) const
		{
			return body->getLinearDamping();
		}

		void PhysXRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			body->setLinearVelocity(physx::PxVec3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
		}

		glm::vec3 PhysXRigidBody::getLinearVelocity(void) const
		{
			const physx::PxVec3 linearVelocity = body->getLinearVelocity();
			return glm::vec3(linearVelocity.x, linearVelocity.y, linearVelocity.z);
		}

		void PhysXRigidBody::setMass(float mass)
		{
			body->setMass(mass);
		}

		float PhysXRigidBody::getMass(void) const
		{
			return body->getMass();
		}

		void PhysXRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
		{
			body->setMassSpaceInertiaTensor(physx::PxVec3(diagonalInertiaTensor.x, diagonalInertiaTensor.y, diagonalInertiaTensor.z));
		}

		glm::vec3 PhysXRigidBody::getDiagonalInertiaTensor(void) const
		{
			const physx::PxVec3 diagonalInertiaTensor = body->getMassSpaceInertiaTensor();
			return glm::vec3(diagonalInertiaTensor.x, diagonalInertiaTensor.y, diagonalInertiaTensor.z);
		}

		void PhysXRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			body->setMaxAngularVelocity(maxAngularVelocity);
		}

		float PhysXRigidBody::getMaxAngularVelocity(void) const
		{
			return body->getMaxAngularVelocity();
		}

		void PhysXRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			body->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
		}

		float PhysXRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return body->getMaxDepenetrationVelocity();
		}

		void PhysXRigidBody::setPosition(const glm::vec3 &position)
		{
			physx::PxTransform transform = body->getGlobalPose();
			transform.p.x = position.x;
			transform.p.y = position.y;
			transform.p.z = position.z;
			body->setGlobalPose(transform);
			body->setKinematicTarget(transform);
		}

		glm::vec3 PhysXRigidBody::getPosition(void) const
		{
			const physx::PxVec3 position = body->getGlobalPose().p;
			return glm::vec3(position.x, position.y, position.z);
		}

		void PhysXRigidBody::setRotation(const glm::quat &rotation)
		{
			physx::PxTransform transform = body->getGlobalPose();
			transform.q.x = rotation.x;
			transform.q.y = rotation.y;
			transform.q.z = rotation.z;
			transform.q.w = rotation.w;
			body->setGlobalPose(transform);
			body->setKinematicTarget(transform);
		}

		glm::quat PhysXRigidBody::getRotation(void) const
		{
			const physx::PxQuat rotation = body->getGlobalPose().q;
			return glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		}

		void PhysXRigidBody::affectByGravity(bool mustBeAffectedByGravity)
		{
			body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !mustBeAffectedByGravity);
		}

		bool PhysXRigidBody::isAffectedByGravity(void) const
		{
			return !body->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
		}

		void PhysXRigidBody::setAsKinematic(bool mustBeKinematic)
		{
			body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, mustBeKinematic);
			if (mustBeKinematic)
			{
				const glm::vec3 position = getPosition();
				const glm::quat rotation = getRotation();
				body->setKinematicTarget(physx::PxTransform(position.x, position.y, position.z, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));
			}
		}

		bool PhysXRigidBody::isKinematic(void) const
		{
			return body->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC);
		}

		void PhysXRigidBody::setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode)
		{
			body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, collisionDetectionMode == CollisionDetectionMode::Continuous);
		}

		CollisionDetectionMode PhysXRigidBody::getCollisionDetectionMode(void) const
		{
			return body->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eENABLE_CCD) ? CollisionDetectionMode::Continuous : CollisionDetectionMode::Discrete;
		}

		void PhysXRigidBody::addForce(const glm::vec3 &force, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case ForceMode::Acceleration:
					body->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					body->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					body->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					body->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eVELOCITY_CHANGE);
					break;
				default:
					break;
			}
		}

		void PhysXRigidBody::addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case ForceMode::Acceleration:
					physx::PxRigidBodyExt::addForceAtPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					physx::PxRigidBodyExt::addForceAtPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					physx::PxRigidBodyExt::addForceAtPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					physx::PxRigidBodyExt::addForceAtPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eVELOCITY_CHANGE);
					break;
				default:
					break;
			}
		}

		void PhysXRigidBody::addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case ForceMode::Acceleration:
					physx::PxRigidBodyExt::addForceAtLocalPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					physx::PxRigidBodyExt::addForceAtLocalPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					physx::PxRigidBodyExt::addForceAtLocalPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					physx::PxRigidBodyExt::addForceAtLocalPos(*body, physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eVELOCITY_CHANGE);
					break;
				default:
					break;
			}
		}

		void PhysXRigidBody::addTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case ForceMode::Acceleration:
					body->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					body->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					body->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					body->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eVELOCITY_CHANGE);
					break;
				default:
					break;
			}
		}

		glm::vec3 PhysXRigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
		{
			const physx::PxVec3 velocity = physx::PxRigidBodyExt::getVelocityAtPos(*body, physx::PxVec3(position.x, position.y, position.z));
			return glm::vec3(velocity.x, velocity.y, velocity.z);
		}

		glm::vec3 PhysXRigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
		{
			const physx::PxVec3 velocity = physx::PxRigidBodyExt::getVelocityAtOffset(*body, physx::PxVec3(position.x, position.y, position.z));
			return glm::vec3(velocity.x, velocity.y, velocity.z);
		}
	}
}