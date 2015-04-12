#pragma once

#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXMeshCollider final : public MeshColliderInterface
		{
		public:
			// Constructors
			PhysXMeshCollider(void) = delete;

			PhysXMeshCollider(WorldInterface *world);

			PhysXMeshCollider(const PhysXMeshCollider &) = delete;

			// Assignment Operators
			PhysXMeshCollider &operator=(const PhysXMeshCollider &) = delete;

		protected:
			// Destructor
			~PhysXMeshCollider(void) = default;
		};
	}
}