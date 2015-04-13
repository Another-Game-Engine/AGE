#pragma once

#include "MeshColliderInterface.hpp"
#include "PhysXCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class PhysXMeshCollider final : public MeshColliderInterface, public PhysXCollider
		{
		public:
			// Constructors
			PhysXMeshCollider(void) = delete;

			PhysXMeshCollider(WorldInterface *world, void *&data);

			PhysXMeshCollider(const PhysXMeshCollider &) = delete;

			// Assignment Operators
			PhysXMeshCollider &operator=(const PhysXMeshCollider &) = delete;

		protected:
			// Destructor
			~PhysXMeshCollider(void) = default;
		};
	}
}

#pragma warning(pop)