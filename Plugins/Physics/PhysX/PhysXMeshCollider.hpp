#pragma once

#include "Physics/MeshColliderInterface.hpp"
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

			PhysXMeshCollider(WorldInterface *world, const std::string &mesh, Private::GenericData *data);

			PhysXMeshCollider(const PhysXMeshCollider &) = delete;

			// Assignment Operators
			PhysXMeshCollider &operator=(const PhysXMeshCollider &) = delete;
			virtual inline PhysXMeshCollider    *getAsPhysXMeshCollider() { return this; }
		private:
			// Static Methods
			static physx::PxConvexMesh *CreateConvexMesh(WorldInterface *world, const std::string &meshPath);

			static physx::PxTriangleMesh *CreateTriangleMesh(WorldInterface *world, const std::string &meshPath);

			// Destructor
			~PhysXMeshCollider(void) = default;

			// Inherited Methods
			void scale(const glm::vec3 &scaling) override final;

			void setMesh(const std::string &mesh) override final;

			void updateShape(void) override final;
		};
	}
}

#pragma warning(pop)