#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class EngineTypes : std::uint8_t
		{
			Bullet,
			PhysX,
			Null
		};
	}
}