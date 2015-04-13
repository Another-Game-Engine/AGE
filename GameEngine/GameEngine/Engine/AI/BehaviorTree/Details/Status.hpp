#pragma once

#include <cstdint>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			enum class Status : std::uint8_t
			{
				Success,
				Failure,
				Running
			};
		}
	}
}
