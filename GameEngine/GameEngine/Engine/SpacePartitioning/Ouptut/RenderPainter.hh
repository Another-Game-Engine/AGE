#pragma once

#include <vector>
#include <Render/Key.hh>

namespace AGE
{

	class Vertices;
	class Properties;
	class Painter;

	struct RenderPainter
	{
		std::vector<std::pair<Key<Vertices>, Key<Properties>>> vertices;
		Key<Painter> painter;
	};

}