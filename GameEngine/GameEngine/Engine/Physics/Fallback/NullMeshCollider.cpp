#pragma once

#include "NullMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullMeshCollider::NullMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, IsConvexByDefault(), data), NullCollider(world, data)
		{
			return;
		}
		
		// Inherited Methods
		void NullMeshCollider::scale(const glm::vec3 &scaling)
		{
			return;
		}

		void NullMeshCollider::updateShape(void)
		{
			return;
		}
	}
}