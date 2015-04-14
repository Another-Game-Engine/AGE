#pragma once

#include "CapsuleColliderInterface.hpp"
#include "PhysXCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class PhysXCapsuleCollider final : public CapsuleColliderInterface, public PhysXCollider
		{
			// Friendships
			friend ObjectPool < PhysXCapsuleCollider >;

		public:
			// Constructors
			PhysXCapsuleCollider(void) = delete;

			PhysXCapsuleCollider(WorldInterface *world, Private::GenericData *data);

			PhysXCapsuleCollider(const PhysXCapsuleCollider &) = delete;

			// Assignment Operators
			PhysXCapsuleCollider &operator=(const PhysXCapsuleCollider &) = delete;

		private:
			// Destructor
			~PhysXCapsuleCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setHeight(float height) override final;

			float getHeight(void) const override final;

			void setRadius(float radius) override final;

			float getRadius(void) const override final;
		};
	}
}

#pragma warning(pop)