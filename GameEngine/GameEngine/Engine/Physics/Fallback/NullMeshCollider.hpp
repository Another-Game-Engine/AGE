#pragma once

#include <Physics/MeshColliderInterface.hpp>
#include <Physics/Fallback/NullCollider.hpp>

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class NullMeshCollider final : public MeshColliderInterface, public NullCollider
		{
			// Friendships
			friend ObjectPool < NullMeshCollider >;

		public:
			// Constructors
			NullMeshCollider(void) = delete;

			NullMeshCollider(WorldInterface *world, Private::GenericData *data);

			NullMeshCollider(const NullMeshCollider &) = delete;

			// Assignment Operators
			NullMeshCollider &operator=(const NullMeshCollider &) = delete;

		protected:
			// Destructor
			~NullMeshCollider(void) = default;
		};
	}
}

#pragma warning(pop)