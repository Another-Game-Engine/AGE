#pragma once

#include "SphereColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXSphereCollider final : public SphereColliderInterface
		{
		public:
			// Constructors
			PhysXSphereCollider(void) = delete;

			PhysXSphereCollider(WorldInterface *world);

			PhysXSphereCollider(const PhysXSphereCollider &) = delete;

			// Assignment Operators
			PhysXSphereCollider &operator=(const PhysXSphereCollider &) = delete;

		protected:
			// Destructor
			~PhysXSphereCollider(void) = default;
		};
	}
}