#pragma once

#include "Physics/BoxColliderInterface.hpp"
#include "PhysXCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class PhysXBoxCollider final : public BoxColliderInterface, public PhysXCollider
		{
			// Friendships
			friend ObjectPool < PhysXBoxCollider >;

		public:
			// Constructors
			PhysXBoxCollider(void) = delete;

			PhysXBoxCollider(WorldInterface *world, Private::GenericData *data);

			PhysXBoxCollider(const PhysXBoxCollider &) = delete;

			// Assignment Operators
			PhysXBoxCollider &operator=(const PhysXBoxCollider &) = delete;

			virtual inline PhysXBoxCollider     *getAsPhysXBoxCollider() { return this; }
		private:
			// Destructor
			~PhysXBoxCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setSize(const glm::vec3 &size) override final;

			glm::vec3 getSize(void) const override final;

			void scale(const glm::vec3 &scaling) override final;
		};
	}
}

#pragma warning(pop)