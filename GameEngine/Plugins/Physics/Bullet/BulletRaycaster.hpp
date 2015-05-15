#pragma once

#include "RaycasterInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletRaycaster final : public RaycasterInterface
		{
		public:
			// Constructors
			BulletRaycaster(void) = delete;

			BulletRaycaster(WorldInterface *world);

			BulletRaycaster(const BulletRaycaster &) = delete;

			// Assignment Operators
			BulletRaycaster &operator=(const BulletRaycaster &) = delete;

		private:
			// Destructor
			~BulletRaycaster(void) = default;
		};
	}
}