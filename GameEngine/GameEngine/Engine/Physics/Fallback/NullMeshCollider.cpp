#pragma once

#include "NullMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullMeshCollider::NullMeshCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), MeshColliderInterface(world, data), NullCollider(world, data)
		{
			return;
		}
	}
}