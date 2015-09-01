#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class FilterGroup : std::uint32_t
		{
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
			GroupXVII = GroupXVI << 1,
			GroupXVIII = GroupXVII << 1,
			GroupXIX = GroupXVIII << 1,
			GroupXX = GroupXIX << 1,
			GroupXXI = GroupXX << 1,
			GroupXXII = GroupXXI << 1,
			GroupXXIII = GroupXXII << 1,
			GroupXXIV = GroupXXIII << 1,
			GroupXXV = GroupXXIV << 1,
			GroupXXVI = GroupXXV << 1,
			GroupXXVII = GroupXXVI << 1,
			GroupXXVIII = GroupXXVII << 1,
			GroupXXIX = GroupXXVIII << 1,
			GroupXXX = GroupXXIX << 1,
			GroupXXXI = GroupXXX << 1,
			GroupXXXII = GroupXXXI << 1
		};

		using LayerMask = FilterGroup;

		LayerMask operator&(LayerMask lhs, LayerMask rhs);

		LayerMask operator|(LayerMask lhs, LayerMask rhs);

		LayerMask operator^(LayerMask lhs, LayerMask rhs);

		LayerMask &operator&=(LayerMask &lhs, LayerMask rhs);

		LayerMask &operator|=(LayerMask &lhs, LayerMask rhs);

		LayerMask operator^=(LayerMask lhs, LayerMask rhs);

		LayerMask operator~(LayerMask lhs);

		static const LayerMask None = LayerMask();

		static const LayerMask All = ~None;

		static const LayerMask DefaultLayerMask = All;
	}
}

#include "FilterGroup.inl"
