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
		std::vector<Key<Vertices>> vertices;
		std::vector<Properties> properties;
		Key<Painter> painter;
	};

}