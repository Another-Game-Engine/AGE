#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class EngineType : std::uint8_t
		{
			Bullet,
			PhysX,
			Null
		};
	}
}