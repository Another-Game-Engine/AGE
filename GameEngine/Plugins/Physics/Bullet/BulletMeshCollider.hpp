#pragma once

#include "MeshColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletMeshCollider final : public MeshColliderInterface
		{
		public:
			// Constructors
			BulletMeshCollider(void) = delete;

			BulletMeshCollider(WorldInterface *world);

			BulletMeshCollider(const BulletMeshCollider &) = delete;

			// Assignment Operators
			BulletMeshCollider &operator=(const BulletMeshCollider &) = delete;

		protected:
			// Destructor
			~BulletMeshCollider(void) = default;
		};
	}
}