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

			NullMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data);

			NullMeshCollider(const NullMeshCollider &) = delete;

			// Assignment Operators
			NullMeshCollider &operator=(const NullMeshCollider &) = delete;

		private:
			// Destructor
			~NullMeshCollider(void) = default;

			// Inherited Methods
			void scale(const glm::vec3 &scaling) override final;
		};
	}
}

#pragma warning(pop)