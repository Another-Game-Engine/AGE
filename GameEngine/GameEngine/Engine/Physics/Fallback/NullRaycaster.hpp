#pragma once

#include <Physics/RaycasterInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullRaycaster final : public RaycasterInterface
		{
		public:
			// Constructors
			NullRaycaster(void) = delete;

			NullRaycaster(WorldInterface *world);

			NullRaycaster(const NullRaycaster &) = delete;

			// Assignment Operators
			NullRaycaster &operator=(const NullRaycaster &) = delete;

		private:
			// Destructor
			~NullRaycaster(void) = default;
		};
	}
}