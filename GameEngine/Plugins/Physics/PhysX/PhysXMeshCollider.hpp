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

			PhysXMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data);

			PhysXMeshCollider(const PhysXMeshCollider &) = delete;

			// Assignment Operators
			PhysXMeshCollider &operator=(const PhysXMeshCollider &) = delete;

		private:
			// Static Methods
			static physx::PxConvexMesh *CreateConvexMesh(WorldInterface *world, std::shared_ptr<MeshInstance> mesh);

			static physx::PxTriangleMesh *CreateTriangleMesh(WorldInterface *world, std::shared_ptr<MeshInstance> mesh);

			// Destructor
			~PhysXMeshCollider(void) = default;

			// Inherited Methods
			void scale(const glm::vec3 &scaling) override final;

			void setMesh(std::shared_ptr<MeshInstance> mesh) override final;

			void updateShape(void) override final;
		};
	}
}

#pragma warning(pop)