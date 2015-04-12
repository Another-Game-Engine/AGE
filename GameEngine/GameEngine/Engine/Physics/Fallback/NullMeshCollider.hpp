#pragma once

#include <Physics/MeshColliderInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullMeshCollider final : public MeshColliderInterface
		{
		public:
			// Constructors
			NullMeshCollider(void) = delete;

			NullMeshCollider(WorldInterface *world);

			NullMeshCollider(const NullMeshCollider &) = delete;

			// Assignment Operators
			NullMeshCollider &operator=(const NullMeshCollider &) = delete;

		protected:
			// Destructor
			~NullMeshCollider(void) = default;
		};
	}
}