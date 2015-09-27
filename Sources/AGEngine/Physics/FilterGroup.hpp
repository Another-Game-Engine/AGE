#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class FilterGroup : std::uint32_t
		{
			None = 0,
			GroupI = 1,
			GroupII = GroupI << 1,
			GroupIII = GroupII << 1,
			GroupIV = GroupIII << 1,
			GroupV = GroupIV << 1,
			GroupVI = GroupV << 1,
			GroupVII = GroupVI << 1,
			GroupVIII = GroupVII << 1,
			GroupIX = GroupVIII << 1,
			GroupX = GroupIX << 1,
			GroupXI = GroupX << 1,
			GroupXII = GroupXI << 1,
			GroupXIII = GroupXII << 1,
			GroupXIV = GroupXIII << 1,
			GroupXV = GroupXIV << 1,
			GroupXVI = GroupXV << 1,
			All = ~None,
			Default = GroupI
		};

		using LayerMask = FilterGroup;

		LayerMask operator&(LayerMask lhs, LayerMask rhs);

		LayerMask operator|(LayerMask lhs, LayerMask rhs);

		LayerMask operator^(LayerMask lhs, LayerMask rhs);

		LayerMask &operator&=(LayerMask &lhs, LayerMask rhs);

		LayerMask &operator|=(LayerMask &lhs, LayerMask rhs);

		LayerMask operator^=(LayerMask &lhs, LayerMask rhs);

		LayerMask operator~(LayerMask lhs);
	}
}

#include "FilterGroup.inl"
