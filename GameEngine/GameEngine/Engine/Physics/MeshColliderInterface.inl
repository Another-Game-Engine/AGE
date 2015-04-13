#pragma once

#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline MeshColliderInterface::MeshColliderInterface(WorldInterface *world, void *&data)
			: ColliderInterface(world, data)
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