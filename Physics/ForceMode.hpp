#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class ForceMode : std::uint8_t
		{
			Force,
			Acceleration,
			Impulse,
			VelocityChange
		};
	}
}