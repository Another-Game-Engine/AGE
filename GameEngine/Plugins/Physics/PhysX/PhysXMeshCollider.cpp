#pragma once

#include "PhysXMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		physx::PxConvexMesh *PhysXMeshCollider::CreateConvexMesh(WorldInterface *world, std::shared_ptr<MeshInstance> mesh)
		{
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh == nullptr)
			{
				return nullptr;
			}
			std::vector<physx::PxVec3> points;
			for (SubMeshData &subMesh : mesh->meshData->subMeshs)
			{
				for (const glm::vec3 &position : subMesh.positions)
				{
					points.push_back(physx::PxVec3(position.x, position.y, position.z));
				}
			}
			physx::PxConvexMeshDesc meshDesciption;
			meshDesciption.points.count = static_cast<physx::PxU32>(points.size());
			meshDesciption.points.stride = static_cast<physx::PxU32>(sizeof(physx::PxVec3));
			meshDesciption.points.data = static_cast<const void *>(&points[0]);
			meshDesciption.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
			meshDesciption.vertexLimit = physx::PxU16(256);
			physx::PxDefaultMemoryOutputStream writeBuffer;
			PhysXPhysics *physics = static_cast<PhysXPhysics *>(world->getPhysics());
			const bool status = physics->getCooking()->cookConvexMesh(meshDesciption, writeBuffer);
			assert(status && "Impossible to create mesh collider");
			if (!status)
			{
				return nullptr;
			}
			physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			return physics->getPhysics()->createConvexMesh(readBuffer);
		}

		// Constructors
		PhysXMeshCollider::PhysXMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, data), PhysXCollider(world, data, static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxConvexMeshGeometry(PhysXMeshCollider::CreateConvexMesh(world, mesh)), *static_cast<const PhysXMaterial *>(getMaterial())->material, true))
		{
			return;
		}

		// Inherited Methods
		void PhysXMeshCollider::scale(const glm::vec3 &scaling)
		{
			physx::PxShape *shape = getShape();
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
			physx::PxConvexMeshGeometry &convexMesh = shape->getGeometry().convexMesh();
			convexMesh.scale.scale.x *= scalingMatrix.column0.magnitude();
			convexMesh.scale.scale.y *= scalingMatrix.column1.magnitude();
			convexMesh.scale.scale.z *= scalingMatrix.column2.magnitude();
			shape->setGeometry(convexMesh);
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}

		void PhysXMeshCollider::setMesh(std::shared_ptr<MeshInstance> mesh)
		{
			MeshColliderInterface::setMesh(mesh);
			assert(mesh != nullptr && "Invalid mesh");
			physx::PxShape *shape = getShape();
			physx::PxConvexMeshGeometry &convexMesh = shape->getGeometry().convexMesh();
			convexMesh.convexMesh = PhysXMeshCollider::CreateConvexMesh(getWorld(), mesh);
			shape->setGeometry(convexMesh);
		}
	}
}