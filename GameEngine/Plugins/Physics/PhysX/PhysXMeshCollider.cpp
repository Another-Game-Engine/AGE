#pragma once

#include "PhysXMeshCollider.hpp"
#include "PhysXWorld.hpp"
#include "../../../GameEngine/Engine/AssetManagement/AssetManager.hh"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXMeshCollider::PhysXMeshCollider(WorldInterface *world, const std::string &mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, IsConvexByDefault(), data), PhysXCollider(world, data, static_cast<PhysXWorld *>(world)->getCollisionShape(mesh, isConvex()))
		{
			getShape()->setMaterials(&static_cast<PhysXMaterial *>(getMaterial())->material, 1);
		}

		// Inherited Methods
		void PhysXMeshCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShape();
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
			if (isConvex())
			{
				physx::PxConvexMeshGeometry &convexMesh = shape->getGeometry().convexMesh();
				convexMesh.scale.scale.x *= scalingMatrix.column0.magnitude();
				convexMesh.scale.scale.y *= scalingMatrix.column1.magnitude();
				convexMesh.scale.scale.z *= scalingMatrix.column2.magnitude();
				shape->setGeometry(convexMesh);
			}
			else
			{
				physx::PxTriangleMeshGeometry &triangleMesh = shape->getGeometry().triangleMesh();
				triangleMesh.scale.scale.x *= scalingMatrix.column0.magnitude();
				triangleMesh.scale.scale.y *= scalingMatrix.column1.magnitude();
				triangleMesh.scale.scale.z *= scalingMatrix.column2.magnitude();
				shape->setGeometry(triangleMesh);
			}
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}

		void PhysXMeshCollider::setMesh(const std::string &mesh)
		{
			MeshColliderInterface::setMesh(mesh);
			static_cast<PhysXCollider *>(this)->updateShape(static_cast<PhysXWorld *>(getWorld())->getCollisionShape(mesh, isConvex()));
		}

		void PhysXMeshCollider::updateShape(void)
		{
			static_cast<PhysXCollider *>(this)->updateShape(static_cast<PhysXWorld *>(getWorld())->getCollisionShape(getMesh(), isConvex()));
		}
	}
}