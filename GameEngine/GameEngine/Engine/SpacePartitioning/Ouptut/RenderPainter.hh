#pragma once

#include <vector>
#include <Render/Key.hh>

namespace AGE
{

	class Vertices;
	class Painter;

	struct RenderPainter
	{
		std::vector<glm::mat4> transformation;
		std::vector<Key<Vertices>> vertices;
		Key<Painter> painter;
	};

}