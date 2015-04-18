#pragma once

#include "PhysXMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXMeshCollider::PhysXMeshCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, data), PhysXCollider(world, data, nullptr)
		{
			// TO_DO --> /!\ PhysXCollider last constructor parameter
			return;
		}

		// Inherited Methods
		void PhysXMeshCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShape();
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			// TO_DO
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}
	}
}