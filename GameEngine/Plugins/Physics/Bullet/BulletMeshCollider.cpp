#pragma once

#include "BulletMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletMeshCollider::BulletMeshCollider(WorldInterface *world)
			: MeshColliderInterface(world)
		{
			return;
		}
	}
}