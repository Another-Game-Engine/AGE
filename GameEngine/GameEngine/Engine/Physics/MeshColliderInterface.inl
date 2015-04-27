#pragma once

#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline MeshColliderInterface::MeshColliderInterface(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
			: ColliderInterface(world, data), mesh(mesh)
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