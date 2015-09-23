#pragma once

#include "PhysXCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXCapsuleCollider::PhysXCapsuleCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), CapsuleColliderInterface(world, data), PhysXCollider(world, data, std::vector<physx::PxShape *>(1, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxCapsuleGeometry(GetDefaultRadius(), GetDefaultHeight() / 2.0f), *static_cast<const PhysXMaterial *>(getMaterial())->material, true)))
		{
			setCenter(GetDefaultCenter());
		}

		// Inherited Methods
		void PhysXCapsuleCollider::setCenter(const glm::vec3 &center)
		{
			getShapes()[0]->setLocalPose(physx::PxTransform(center.x, center.y, center.z));
		}

		glm::vec3 PhysXCapsuleCollider::getCenter(void) const
		{
			const physx::PxVec3 center = getShapes()[0]->getLocalPose().p;
			return glm::vec3(center.x, center.y, center.z);
		}

		void PhysXCapsuleCollider::setHeight(float height)
		{
			getShapes()[0]->getGeometry().capsule().halfHeight = height / 2.0f;
		}

		float PhysXCapsuleCollider::getHeight(void) const
		{
			return 2.0f * getShapes()[0]->getGeometry().capsule().halfHeight;
		}

		void PhysXCapsuleCollider::setRadius(float radius)
		{
			getShapes()[0]->getGeometry().capsule().radius = radius;
		}

		float PhysXCapsuleCollider::getRadius(void) const
		{
			return getShapes()[0]->getGeometry().capsule().radius;
		}

		void PhysXCapsuleCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShapes()[0];
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
			physx::PxCapsuleGeometry &capsule = shape->getGeometry().capsule();
			capsule.halfHeight *= scalingMatrix.column0.magnitude();
			capsule.radius *= std::sqrt(scalingMatrix.column1.magnitude() * scalingMatrix.column2.magnitude());
			shape->setGeometry(capsule);
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}
	}
}