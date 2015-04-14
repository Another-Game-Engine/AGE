#pragma once

#include "PhysXBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXBoxCollider::PhysXBoxCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), PhysXCollider(world, data, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), *static_cast<const PhysXMaterial *>(getMaterial())->material))
		{
			return;
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
	}
}