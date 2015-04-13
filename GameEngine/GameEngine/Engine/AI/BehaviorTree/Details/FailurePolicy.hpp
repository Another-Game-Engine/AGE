#pragma once

#include <cstdint>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			enum class FailurePolicy : std::uint8_t
			{
				FailOnOne,
				FailOnAll
			};
		}
	}
}
