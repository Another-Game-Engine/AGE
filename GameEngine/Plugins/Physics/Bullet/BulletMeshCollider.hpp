#pragma once

#include <vector>

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
			// Friendships
			friend ObjectPool < BulletMeshCollider > ;

		public:
			// Constructors
			BulletMeshCollider(void) = delete;

			BulletMeshCollider(WorldInterface *world, const std::string &mesh, Private::GenericData *data);

			BulletMeshCollider(const BulletMeshCollider &) = delete;

			// Assignment Operators
			BulletMeshCollider &operator=(const BulletMeshCollider &) = delete;

		private:
			// Destructor
			~BulletMeshCollider(void) = default;

			// Inherited Methods
			void setMesh(const std::string &mesh) override final;

			void updateShape(void) override final;
		};
	}
}

#pragma warning(pop)