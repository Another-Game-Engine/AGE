#pragma once

#include "MeshColliderInterface.hpp"
#include "BulletCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class BulletMeshCollider final : public MeshColliderInterface, public BulletCollider
		{
		public:
			// Constructors
			BulletMeshCollider(void) = delete;

			BulletMeshCollider(WorldInterface *world, void *&data);

			BulletMeshCollider(const BulletMeshCollider &) = delete;

			// Assignment Operators
			BulletMeshCollider &operator=(const BulletMeshCollider &) = delete;

		protected:
			// Destructor
			~BulletMeshCollider(void) = default;
		};
	}
}

#pragma warning(pop)