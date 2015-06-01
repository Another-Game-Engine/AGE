#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class CollisionDetectionMode : std::uint8_t
		{
			Discrete,
			Continuous
		};

		static const char *CollisionDetectionModeStr[2] = {
			"Discrete"
			, "Continuous"
		};
	}
}