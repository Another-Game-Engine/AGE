#pragma once

#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		inline bool MeshColliderInterface::IsConvexByDefault(void)
		{
			return true;
		}

		// Constructors
		inline MeshColliderInterface::MeshColliderInterface(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, bool mustBeConvex, Private::GenericData *data)
			: ColliderInterface(world, data), mesh(mesh), convex(mustBeConvex)
		{
			assert(mesh != nullptr && "Invalid mesh");
		}

		// Methods
		inline std::shared_ptr<MeshInstance> MeshColliderInterface::getMesh(void)
		{
			return mesh;
		}

		inline std::shared_ptr<const MeshInstance> MeshColliderInterface::getMesh(void) const
		{
			return mesh;
		}

		inline bool MeshColliderInterface::isConvex(void) const
		{
			return convex;
		}

		inline void MeshColliderInterface::setAsConvex(bool mustBeConvex)
		{
			if (mustBeConvex != convex)
			{
				convex = mustBeConvex;
				updateShape();
			}
		}

		// Virtual Methods
		inline void MeshColliderInterface::setMesh(std::shared_ptr<MeshInstance> mesh)
		{
			this->mesh = mesh;
		}

		// Inherited Methods
		inline ColliderType MeshColliderInterface::getColliderType(void) const
		{
			return ColliderType::Mesh;
		}
	}
}