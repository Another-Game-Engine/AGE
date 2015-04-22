#include "PhysXRigidBody.hpp"
#include "PhysXWorld.hpp"
#include "PhysXCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXRigidBody::PhysXRigidBody(PhysXWorld *world, Private::GenericData *data)
			: RigidBodyInterface(world, data)
		{
			if (getData()->data == nullptr)
			{
				getData()->data = static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createRigidDynamic(physx::PxTransform(physx::PxIdentity));
				assert(getData()->data != nullptr && "Impossible to create actor");
				physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
				world->getScene()->addActor(*body);
				body->userData = this;
			}
			else
			{
				physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
				PhysXCollider *collider = static_cast<PhysXCollider *>(body->userData);
				collider->rigidBody = this;
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
		PhysXRigidBody::~PhysXRigidBody(void)
		{
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			PhysXCollider *collider = static_cast<PhysXCollider *>(body->userData);
			if (collider == nullptr || static_cast<void *>(collider) == this)
			{
				static_cast<PhysXWorld *>(getWorld())->getScene()->removeActor(*body);
				body->release();
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
		void PhysXRigidBody::setAngularDrag(float angularDrag)
		{
			getDataAs<physx::PxRigidDynamic>()->setAngularDamping(angularDrag);
		}

		float PhysXRigidBody::getAngularDrag(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getAngularDamping();
		}

		void PhysXRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			getDataAs<physx::PxRigidDynamic>()->setAngularVelocity(physx::PxVec3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
		}

		glm::vec3 PhysXRigidBody::getAngularVelocity(void) const
		{
			const physx::PxVec3 angularVelocity = getDataAs<physx::PxRigidDynamic>()->getAngularVelocity();
			return glm::vec3(angularVelocity.x, angularVelocity.y, angularVelocity.z);
		}

		void PhysXRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			getDataAs<physx::PxRigidDynamic>()->setCMassLocalPose(physx::PxTransform(centerOfMass.x, centerOfMass.y, centerOfMass.z));
		}

		glm::vec3 PhysXRigidBody::getCenterOfMass(void) const
		{
			const physx::PxVec3 centerOfMass = getDataAs<physx::PxRigidDynamic>()->getCMassLocalPose().p;
			return glm::vec3(centerOfMass.x, centerOfMass.y, centerOfMass.z);
		}

		void PhysXRigidBody::setLinearDrag(float linearDrag)
		{
			getDataAs<physx::PxRigidDynamic>()->setLinearDamping(linearDrag);
		}

		float PhysXRigidBody::getLinearDrag(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getLinearDamping();
		}

		void PhysXRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			getDataAs<physx::PxRigidDynamic>()->setLinearVelocity(physx::PxVec3(linearVelocity.x, linearVelocity.y, linearVelocity.z));
		}

		glm::vec3 PhysXRigidBody::getLinearVelocity(void) const
		{
			const physx::PxVec3 linearVelocity = getDataAs<physx::PxRigidDynamic>()->getLinearVelocity();
			return glm::vec3(linearVelocity.x, linearVelocity.y, linearVelocity.z);
		}

		void PhysXRigidBody::setMass(float mass)
		{
			getDataAs<physx::PxRigidDynamic>()->setMass(mass);
		}

		float PhysXRigidBody::getMass(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getMass();
		}

		void PhysXRigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
		{
			getDataAs<physx::PxRigidDynamic>()->setMassSpaceInertiaTensor(physx::PxVec3(diagonalInertiaTensor.x, diagonalInertiaTensor.y, diagonalInertiaTensor.z));
		}

		glm::vec3 PhysXRigidBody::getDiagonalInertiaTensor(void) const
		{
			const physx::PxVec3 diagonalInertiaTensor = getDataAs<physx::PxRigidDynamic>()->getMassSpaceInertiaTensor();
			return glm::vec3(diagonalInertiaTensor.x, diagonalInertiaTensor.y, diagonalInertiaTensor.z);
		}

		void PhysXRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			getDataAs<physx::PxRigidDynamic>()->setMaxAngularVelocity(maxAngularVelocity);
		}

		float PhysXRigidBody::getMaxAngularVelocity(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getMaxAngularVelocity();
		}

		void PhysXRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			getDataAs<physx::PxRigidDynamic>()->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
		}

		float PhysXRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getMaxDepenetrationVelocity();
		}

		void PhysXRigidBody::setPosition(const glm::vec3 &position)
		{
			physx::PxRigidDynamic *actor = getDataAs<physx::PxRigidDynamic>();
			physx::PxTransform transform = actor->getGlobalPose();
			transform.p.x = position.x;
			transform.p.y = position.y;
			transform.p.z = position.z;
			actor->setGlobalPose(transform);
			if (isKinematic())
			{
				actor->setKinematicTarget(transform);
			}
		}

		glm::vec3 PhysXRigidBody::getPosition(void) const
		{
			const physx::PxVec3 position = getDataAs<physx::PxRigidDynamic>()->getGlobalPose().p;
			return glm::vec3(position.x, position.y, position.z);
		}

		void PhysXRigidBody::setRotation(const glm::quat &rotation)
		{
			physx::PxRigidDynamic *actor = getDataAs<physx::PxRigidDynamic>();
			physx::PxTransform transform = actor->getGlobalPose();
			transform.q.x = rotation.x;
			transform.q.y = rotation.y;
			transform.q.z = rotation.z;
			transform.q.w = rotation.w;
			actor->setGlobalPose(transform);
			if (isKinematic())
			{
				actor->setKinematicTarget(transform);
			}
		}

		glm::quat PhysXRigidBody::getRotation(void) const
		{
			const physx::PxQuat rotation = getDataAs<physx::PxRigidDynamic>()->getGlobalPose().q;
			return glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		}

		void PhysXRigidBody::affectByGravity(bool mustBeAffectedByGravity)
		{
			getDataAs<physx::PxRigidDynamic>()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !mustBeAffectedByGravity);
		}

		bool PhysXRigidBody::isAffectedByGravity(void) const
		{
			return !getDataAs<physx::PxRigidDynamic>()->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
		}

		void PhysXRigidBody::setAsKinematic(bool mustBeKinematic)
		{
			physx::PxRigidDynamic *actor = getDataAs<physx::PxRigidDynamic>();
			actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, mustBeKinematic);
			if (mustBeKinematic)
			{
				const glm::vec3 position = getPosition();
				const glm::quat rotation = getRotation();
				actor->setKinematicTarget(physx::PxTransform(position.x, position.y, position.z, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));
			}
		}

		bool PhysXRigidBody::isKinematic(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC);
		}

		void PhysXRigidBody::setCollisionDetectionMode(CollisionDetectionMode collisionDetectionMode)
		{
			getDataAs<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, collisionDetectionMode == CollisionDetectionMode::Continuous);
		}

		CollisionDetectionMode PhysXRigidBody::getCollisionDetectionMode(void) const
		{
			return getDataAs<physx::PxRigidDynamic>()->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eENABLE_CCD) ? CollisionDetectionMode::Continuous : CollisionDetectionMode::Discrete;
		}

		void PhysXRigidBody::addForce(const glm::vec3 &force, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case ForceMode::Acceleration:
					getDataAs<physx::PxRigidDynamic>()->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					getDataAs<physx::PxRigidDynamic>()->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					getDataAs<physx::PxRigidDynamic>()->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					getDataAs<physx::PxRigidDynamic>()->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eVELOCITY_CHANGE);
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
					physx::PxRigidBodyExt::addForceAtPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					physx::PxRigidBodyExt::addForceAtPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					physx::PxRigidBodyExt::addForceAtPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					physx::PxRigidBodyExt::addForceAtPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eVELOCITY_CHANGE);
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
					physx::PxRigidBodyExt::addForceAtLocalPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					physx::PxRigidBodyExt::addForceAtLocalPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					physx::PxRigidBodyExt::addForceAtLocalPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					physx::PxRigidBodyExt::addForceAtLocalPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(force.x, force.y, force.z), physx::PxVec3(position.x, position.y, position.z), physx::PxForceMode::eVELOCITY_CHANGE);
					break;
				default:
					break;
			}
		}

		void PhysXRigidBody::addAbsoluteTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			switch (forceMode)
			{
				case ForceMode::Acceleration:
					getDataAs<physx::PxRigidDynamic>()->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eACCELERATION);
					break;
				case ForceMode::Force:
					getDataAs<physx::PxRigidDynamic>()->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eFORCE);
					break;
				case ForceMode::Impulse:
					getDataAs<physx::PxRigidDynamic>()->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eIMPULSE);
					break;
				case ForceMode::VelocityChange:
					getDataAs<physx::PxRigidDynamic>()->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eVELOCITY_CHANGE);
					break;
				default:
					break;
			}
		}

		void PhysXRigidBody::addRelativeTorque(const glm::vec3 &torque, ForceMode forceMode)
		{
			const physx::PxVec3 absoluteTorque = getDataAs<physx::PxRigidDynamic>()->getGlobalPose().transform(physx::PxVec3(torque.x, torque.y, torque.z));
			addAbsoluteTorque(glm::vec3(absoluteTorque.x, absoluteTorque.y, absoluteTorque.z), forceMode);
		}

		glm::vec3 PhysXRigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
		{
			const physx::PxVec3 velocity = physx::PxRigidBodyExt::getVelocityAtPos(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(position.x, position.y, position.z));
			return glm::vec3(velocity.x, velocity.y, velocity.z);
		}

		glm::vec3 PhysXRigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
		{
			const physx::PxVec3 velocity = physx::PxRigidBodyExt::getVelocityAtOffset(*getDataAs<physx::PxRigidDynamic>(), physx::PxVec3(position.x, position.y, position.z));
			return glm::vec3(velocity.x, velocity.y, velocity.z);
		}
	}
}