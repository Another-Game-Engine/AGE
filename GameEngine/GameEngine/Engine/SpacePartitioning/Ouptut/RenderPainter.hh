#pragma once

#include <vector>
#include <Render/Key.hh>

namespace AGE
{

	class Vertices;
	class Painter;

	struct RenderPainter
	{
		std::vector<Key<Vertices>> vertices;
		Key<Painter> painter;
	};

}