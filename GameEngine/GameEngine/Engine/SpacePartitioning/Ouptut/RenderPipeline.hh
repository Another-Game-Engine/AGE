#pragma once

#include <Render/Key.hh>
#include <unordered_map>

namespace AGE
{
	struct RenderPainter;

	struct RenderPipeline
	{
		std::unordered_map<size_t, RenderPainter> keys;
	};

}