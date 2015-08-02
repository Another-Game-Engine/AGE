#pragma once

#include "BulletMeshCollider.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletMeshCollider::BulletMeshCollider(WorldInterface *world, const std::string &mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, IsConvexByDefault(), data), BulletCollider(world, data, static_cast<BulletWorld *>(world)->getCollisionShape(mesh, isConvex()))
		{
			return;
		}

		// Inherited Methods
		void BulletMeshCollider::setMesh(const std::string &mesh)
		{
			MeshColliderInterface::setMesh(mesh);
			static_cast<BulletCollider *>(this)->updateShape(static_cast<BulletWorld *>(getWorld())->getCollisionShape(mesh, isConvex()));
		}

		void BulletMeshCollider::updateShape(void)
		{
			static_cast<BulletCollider *>(this)->updateShape(static_cast<BulletWorld *>(getWorld())->getCollisionShape(getMesh(), isConvex()));
		}
	}
}