#pragma once

#include "CapsuleColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXCapsuleCollider final : public CapsuleColliderInterface
		{
		public:
			// Constructors
			PhysXCapsuleCollider(void) = delete;

			PhysXCapsuleCollider(WorldInterface *world);

			PhysXCapsuleCollider(const PhysXCapsuleCollider &) = delete;

			// Assignment Operators
			PhysXCapsuleCollider &operator=(const PhysXCapsuleCollider &) = delete;

		protected:
			// Destructor
			~PhysXCapsuleCollider(void) = default;
		};
	}
}