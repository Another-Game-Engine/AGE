#pragma once

#include "PhysXSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXSphereCollider::PhysXSphereCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), SphereColliderInterface(world, data), PhysXCollider(world, data, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxSphereGeometry(1.0f), *static_cast<const PhysXMaterial *>(getMaterial())->material))
		{
			return;
		}

		// Inherited Methods
		void PhysXSphereCollider::setCenter(const glm::vec3 &center)
		{
			getShape()->setLocalPose(physx::PxTransform(center.x, center.y, center.z));
		}

		glm::vec3 PhysXSphereCollider::getCenter(void) const
		{
			const physx::PxVec3 center = getShape()->getLocalPose().p;
			return glm::vec3(center.x, center.y, center.z);
		}

		void PhysXSphereCollider::setRadius(float radius)
		{
			getShape()->getGeometry().sphere().radius = radius;
		}

		float PhysXSphereCollider::getRadius(void) const
		{
			return getShape()->getGeometry().sphere().radius;
		}
	}
}