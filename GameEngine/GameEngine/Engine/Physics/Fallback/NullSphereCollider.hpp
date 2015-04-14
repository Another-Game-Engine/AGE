#pragma once

#include <Physics/SphereColliderInterface.hpp>
#include <Physics/Fallback/NullCollider.hpp>

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class NullSphereCollider final : public SphereColliderInterface, public NullCollider
		{
			// Friendships
			friend ObjectPool < NullSphereCollider >;

		public:
			// Constructors
			NullSphereCollider(void) = delete;

			NullSphereCollider(WorldInterface *world, Private::GenericData *data);

			NullSphereCollider(const NullSphereCollider &) = delete;

			// Assignment Operators
			NullSphereCollider &operator=(const NullSphereCollider &) = delete;

		private:
			// Destructor
			~NullSphereCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setRadius(float radius) override final;

			float getRadius(void) const override final;
		};
	}
}

#pragma warning(pop)