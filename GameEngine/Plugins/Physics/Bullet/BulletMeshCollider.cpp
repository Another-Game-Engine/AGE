#pragma once

#include "BulletMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletMeshCollider::BulletMeshCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, data), BulletCollider(world, data)
		{
			return;
		}
	}
}