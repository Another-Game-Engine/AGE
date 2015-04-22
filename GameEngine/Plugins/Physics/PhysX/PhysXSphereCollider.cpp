#pragma once

#include "PhysXSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXSphereCollider::PhysXSphereCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), SphereColliderInterface(world, data), PhysXCollider(world, data, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxSphereGeometry(GetDefaultRadius()), *static_cast<const PhysXMaterial *>(getMaterial())->material, true))
		{
			setCenter(GetDefaultCenter());
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

		void PhysXSphereCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShape();
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			shape->getGeometry().sphere().radius *= std::pow(std::abs(realScaling.x * realScaling.y * realScaling.z), 1.0f / 3.0f);
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}
	}
}