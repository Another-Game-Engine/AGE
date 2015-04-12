#pragma once

#include "BoxColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXBoxCollider final : public BoxColliderInterface
		{
		public:
			// Constructors
			PhysXBoxCollider(void) = delete;

			PhysXBoxCollider(WorldInterface *world);

			PhysXBoxCollider(const PhysXBoxCollider &) = delete;

			// Assignment Operators
			PhysXBoxCollider &operator=(const PhysXBoxCollider &) = delete;

		protected:
			// Destructor
			~PhysXBoxCollider(void) = default;
		};
	}
}