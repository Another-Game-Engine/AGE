#pragma once

#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline MeshColliderInterface::MeshColliderInterface(WorldInterface *world)
			: ColliderInterface(world)
		{
			return;
		}

		// Inherited Methods
		inline ColliderType MeshColliderInterface::getColliderType(void) const
		{
			return ColliderType::Mesh;
		}
	}
}