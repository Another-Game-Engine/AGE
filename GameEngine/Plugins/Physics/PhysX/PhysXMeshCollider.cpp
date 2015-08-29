#pragma once

#include "PhysXMeshCollider.hpp"
#include "../../../GameEngine/Engine/AssetManagement/AssetManager.hh"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		physx::PxConvexMesh *PhysXMeshCollider::CreateConvexMesh(WorldInterface *world, const std::string &meshPath)
		{
			std::shared_ptr<MeshInstance> mesh = world->getAssetManager()->getMesh(meshPath + ".sage");

			if (mesh == nullptr &&
				world->getAssetManager()->loadMesh(meshPath + ".sage", meshPath) == false)
			{
				return nullptr;
			}
			mesh = world->getAssetManager()->getMesh(meshPath + ".sage");
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

		physx::PxTriangleMesh *PhysXMeshCollider::CreateTriangleMesh(WorldInterface *world, const std::string &meshPath)
		{
			std::shared_ptr<MeshInstance> mesh = world->getAssetManager()->getMesh(meshPath + ".sage");
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh == nullptr)
			{
				return nullptr;
			}
			std::vector<physx::PxVec3> points;
			std::vector<physx::PxU32> indices;
			for (SubMeshData &subMesh : mesh->meshData->subMeshs)
			{
				const std::size_t startIndex = points.size();
				for (const glm::vec3 &position : subMesh.positions)
				{
					points.push_back(physx::PxVec3(position.x, position.y, position.z));
				}
				for (unsigned int indice : subMesh.indices)
				{
					indices.push_back(static_cast<physx::PxU32>(indice + startIndex));
				}
			}
			physx::PxTriangleMeshDesc meshDesciption;
			meshDesciption.points.count = static_cast<physx::PxU32>(points.size());
			meshDesciption.points.stride = static_cast<physx::PxU32>(sizeof(physx::PxVec3));
			meshDesciption.points.data = static_cast<const void *>(&points[0]);
			meshDesciption.triangles.count = static_cast<physx::PxU32>(indices.size());
			meshDesciption.triangles.stride = static_cast<physx::PxU32>(3 * sizeof(physx::PxU32));
			meshDesciption.triangles.data = static_cast<const void *>(&indices[0]);
			physx::PxDefaultMemoryOutputStream writeBuffer;
			PhysXPhysics *physics = static_cast<PhysXPhysics *>(world->getPhysics());
			const bool status = physics->getCooking()->cookTriangleMesh(meshDesciption, writeBuffer);
			assert(status && "Impossible to create mesh collider");
			if (!status)
			{
				return nullptr;
			}
			physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			return physics->getPhysics()->createTriangleMesh(readBuffer);
		}

		// Constructors
		PhysXMeshCollider::PhysXMeshCollider(WorldInterface *world, const std::string &mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, IsConvexByDefault(), data),
			PhysXCollider(world, data,
			IsConvexByDefault() ?
			static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxConvexMeshGeometry(PhysXMeshCollider::CreateConvexMesh(world, mesh)), *static_cast<const PhysXMaterial *>(getMaterial())->material, true) :
			static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxTriangleMeshGeometry(PhysXMeshCollider::CreateTriangleMesh(world, mesh)), *static_cast<const PhysXMaterial *>(getMaterial())->material, true))
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
			assert(!mesh.empty() && "Invalid mesh");
			physx::PxShape *shape = getShape();
			if (isConvex())
			{
				physx::PxConvexMeshGeometry &convexMesh = shape->getGeometry().convexMesh();
				convexMesh.convexMesh = PhysXMeshCollider::CreateConvexMesh(getWorld(), mesh);
				shape->setGeometry(convexMesh);
			}
			else
			{
				physx::PxTriangleMeshGeometry &triangleMesh = shape->getGeometry().triangleMesh();
				triangleMesh.triangleMesh = PhysXMeshCollider::CreateTriangleMesh(getWorld(), mesh);
				shape->setGeometry(triangleMesh);
			}
		}

		void PhysXMeshCollider::updateShape(void)
		{
			WorldInterface *world = getWorld();
			if (isConvex())
			{
				static_cast<PhysXCollider *>(this)->updateShape(static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxConvexMeshGeometry(PhysXMeshCollider::CreateConvexMesh(world, getMesh())), *static_cast<const PhysXMaterial *>(getMaterial())->material, true));
			}
			else
			{
				static_cast<PhysXCollider *>(this)->updateShape(static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createShape(physx::PxTriangleMeshGeometry(PhysXMeshCollider::CreateTriangleMesh(world, getMesh())), *static_cast<const PhysXMaterial *>(getMaterial())->material, true));
			}
		}
	}
}