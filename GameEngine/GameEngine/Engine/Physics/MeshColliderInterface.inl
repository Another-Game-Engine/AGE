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
		inline MeshColliderInterface::MeshColliderInterface(WorldInterface *world, const std::string &mesh, bool mustBeConvex, Private::GenericData *data)
			: ColliderInterface(world, data), mesh(mesh), convex(mustBeConvex)
		{
			assert(!mesh.empty() && "Invalid mesh");
		}

		// Methods
		inline const std::string &MeshColliderInterface::getMesh(void) const
		{
			return mesh;
		}

		inline bool MeshColliderInterface::isConvex(void) const
		{
			return convex;
		}

		inline void MeshColliderInterface::setAsConvex(void)
		{
			if (!isConvex())
			{
				convex = true;
				updateShape();
			}
		}

		inline bool MeshColliderInterface::isConcave(void) const
		{
			return !isConvex();
		}

		inline void MeshColliderInterface::setAsConcave(void)
		{
			if (!isConcave())
			{
				convex = false;
				updateShape();
			}
		}

		// Virtual Methods
		inline void MeshColliderInterface::setMesh(const std::string &mesh)
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