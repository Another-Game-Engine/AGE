#pragma once

#include <cstdint>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			enum class SuccessPolicy : std::uint8_t
			{
				SuccessOnOne,
				SuccessOnAll
			};
		}
	}
}