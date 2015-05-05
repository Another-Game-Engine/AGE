#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class TriggerType : std::uint8_t
		{
			New,
			Persistent,
			Lost
		};
	}
}