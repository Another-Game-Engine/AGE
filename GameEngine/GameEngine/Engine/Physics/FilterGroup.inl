#pragma once

#include "FilterGroup.hpp"

namespace AGE
{
	namespace Physics
	{
		inline LayerMask operator&(LayerMask lhs, LayerMask rhs)
		{
			return static_cast<LayerMask>(static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));
		}

		inline LayerMask operator|(LayerMask lhs, LayerMask rhs)
		{
			return static_cast<LayerMask>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
		}

		inline LayerMask &operator&=(LayerMask &lhs, LayerMask rhs)
		{
			lhs = lhs & rhs;
		}

		inline LayerMask &operator|=(LayerMask &lhs, LayerMask rhs)
		{
			lhs = lhs | rhs;
		}

		inline LayerMask operator~(LayerMask lhs)
		{
			return static_cast<LayerMask>(static_cast<std::uint32_t>(lhs));
		}
	}
}