#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class ContactType : std::uint8_t
		{
			New,
			Persistent,
			Lost
		};
	}
}