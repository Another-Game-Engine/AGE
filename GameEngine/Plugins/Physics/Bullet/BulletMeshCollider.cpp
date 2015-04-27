#pragma once

#include "BulletMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletMeshCollider::BulletMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, data), BulletCollider(world, data, nullptr)
		{
			// TO_DO
		}

		// Inherited Methods
		void BulletMeshCollider::setMesh(std::shared_ptr<MeshInstance> mesh)
		{
			MeshColliderInterface::setMesh(mesh);
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh != nullptr)
			{
				// TO_DO
			}
		}
	}
}