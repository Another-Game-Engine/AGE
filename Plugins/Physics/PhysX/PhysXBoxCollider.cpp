#pragma once

#include "PhysXBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXBoxCollider::PhysXBoxCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), PhysXCollider(world, data, std::vector<physx::PxShape *>(1, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxBoxGeometry(GetDefaultSize().x / 2.0f, GetDefaultSize().y / 2.0f, GetDefaultSize().z / 2.0f), *static_cast<const PhysXMaterial *>(getMaterial())->material, true)))
		{
			setCenter(GetDefaultCenter());
		}

		// Inherited Methods
		void PhysXBoxCollider::setCenter(const glm::vec3 &center)
		{
			getShapes()[0]->setLocalPose(physx::PxTransform(center.x, center.y, center.z));
		}

		glm::vec3 PhysXBoxCollider::getCenter(void) const
		{
			const physx::PxVec3 center = getShapes()[0]->getLocalPose().p;
			return glm::vec3(center.x, center.y, center.z);
		}

		void PhysXBoxCollider::setSize(const glm::vec3 &size)
		{
			getShapes()[0]->getGeometry().box().halfExtents = physx::PxVec3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);
		}

		glm::vec3 PhysXBoxCollider::getSize(void) const
		{
			const physx::PxVec3 size = 2.0f * getShapes()[0]->getGeometry().box().halfExtents;
			return glm::vec3(size.x, size.y, size.z);
		}

		void PhysXBoxCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShapes()[0];
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
			physx::PxBoxGeometry &box = shape->getGeometry().box();
			box.halfExtents.x *= scalingMatrix.column0.magnitude();
			box.halfExtents.y *= scalingMatrix.column1.magnitude();
			box.halfExtents.z *= scalingMatrix.column2.magnitude();
			shape->setGeometry(box);
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}
	}
}