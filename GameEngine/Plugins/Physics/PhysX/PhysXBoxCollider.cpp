#pragma once

#include "PhysXBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXBoxCollider::PhysXBoxCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), PhysXCollider(world, data, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxBoxGeometry(GetDefaultSize().x / 2.0f, GetDefaultSize().y / 2.0f, GetDefaultSize().z / 2.0f), *static_cast<const PhysXMaterial *>(getMaterial())->material, true))
		{
			setCenter(GetDefaultCenter());
		}

		// Inherited Methods
		void PhysXBoxCollider::setCenter(const glm::vec3 &center)
		{
			getShape()->setLocalPose(physx::PxTransform(center.x, center.y, center.z));
		}

		glm::vec3 PhysXBoxCollider::getCenter(void) const
		{
			const physx::PxVec3 center = getShape()->getLocalPose().p;
			return glm::vec3(center.x, center.y, center.z);
		}

		void PhysXBoxCollider::setSize(const glm::vec3 &size)
		{
			getShape()->getGeometry().box().halfExtents = physx::PxVec3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);
		}

		glm::vec3 PhysXBoxCollider::getSize(void) const
		{
			const physx::PxVec3 size = 2.0f * getShape()->getGeometry().box().halfExtents;
			return glm::vec3(size.x, size.y, size.z);
		}

		void PhysXBoxCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShape();
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
			physx::PxVec3 halfExtents = shape->getGeometry().box().halfExtents;
			halfExtents.x *= scalingMatrix.column0.magnitude();
			halfExtents.y *= scalingMatrix.column1.magnitude();
			halfExtents.z *= scalingMatrix.column2.magnitude();
			shape->setGeometry(physx::PxBoxGeometry(halfExtents));
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}
	}
}