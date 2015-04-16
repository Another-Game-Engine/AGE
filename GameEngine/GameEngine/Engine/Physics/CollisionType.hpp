#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class CollisionType : std::uint8_t
		{
			New,
			Persistent,
			Lost
		};
	}
}