#pragma once

#include "RaycasterInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXRaycaster final : public RaycasterInterface
		{
		public:
			// Constructors
			PhysXRaycaster(void) = delete;

			PhysXRaycaster(WorldInterface *world);

			PhysXRaycaster(const PhysXRaycaster &) = delete;

			// Assignment Operators
			PhysXRaycaster &operator=(const PhysXRaycaster &) = delete;

		private:
			// Destructor
			~PhysXRaycaster(void) = default;
		};
	}
}