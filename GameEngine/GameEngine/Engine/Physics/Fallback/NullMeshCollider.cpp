#pragma once

#include "NullMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullMeshCollider::NullMeshCollider(WorldInterface *world)
			: MeshColliderInterface(world)
		{
			return;
		}
	}
}