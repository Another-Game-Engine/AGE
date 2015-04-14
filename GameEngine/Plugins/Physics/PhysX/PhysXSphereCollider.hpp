#pragma once

#include "SphereColliderInterface.hpp"
#include "PhysXCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class PhysXSphereCollider final : public SphereColliderInterface, public PhysXCollider
		{
			// Friendships
			friend ObjectPool < PhysXSphereCollider >;

		public:
			// Constructors
			PhysXSphereCollider(void) = delete;

			PhysXSphereCollider(WorldInterface *world, Private::GenericData *data);

			PhysXSphereCollider(const PhysXSphereCollider &) = delete;

			// Assignment Operators
			PhysXSphereCollider &operator=(const PhysXSphereCollider &) = delete;

		private:
			// Destructor
			~PhysXSphereCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setRadius(float radius) override final;

			float getRadius(void) const override final;
		};
	}
}

#pragma warning(pop)