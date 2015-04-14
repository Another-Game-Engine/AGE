#pragma once

#include "NullMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullMeshCollider::NullMeshCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, data), NullCollider(world, data)
		{
			return;
		}
	}
}