#pragma once

#include <bitset>

namespace AGE
{
	enum RenderModes
	{
		AGE_OPAQUE,
		AGE_SEMI_TRANSPARENT,
		AGE_SKINNED,
		AGE_NORMAL_MAP,
		AGE_OCCLUDER,
		RENDER_MODE_NUMBER,
	};

	typedef std::bitset<RENDER_MODE_NUMBER> RenderModeSet;
}