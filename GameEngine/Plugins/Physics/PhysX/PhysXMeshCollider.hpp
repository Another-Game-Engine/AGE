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
			// Friendships
			friend ObjectPool < PhysXMeshCollider >;

		public:
			// Constructors
			PhysXMeshCollider(void) = delete;

			PhysXMeshCollider(WorldInterface *world, Private::GenericData *data);

			PhysXMeshCollider(const PhysXMeshCollider &) = delete;

			// Assignment Operators
			PhysXMeshCollider &operator=(const PhysXMeshCollider &) = delete;

		private:
			// Destructor
			~PhysXMeshCollider(void) = default;
		};
	}
}

#pragma warning(pop)