#pragma once

#include <vector>

namespace AGE
{
	struct RenderPainter;

	struct RenderPipeline
	{
		std::vector<RenderPainter> painters;
	};

}