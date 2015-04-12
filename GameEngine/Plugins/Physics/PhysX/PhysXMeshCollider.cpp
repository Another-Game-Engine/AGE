#pragma once

#include "PhysXMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXMeshCollider::PhysXMeshCollider(WorldInterface *world)
			: MeshColliderInterface(world)
		{
			return;
		}
	}
}