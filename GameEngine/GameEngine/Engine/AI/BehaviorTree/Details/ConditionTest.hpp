#pragma once

#include <cstdint>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			enum class ConditionTest : std::uint8_t
			{
				LessThan,
				GreaterThan,
				LessOrEqualThan,
				GreaterOrEqualThan,
				Equal,
				NotEqual
			};
		}
	}
}
