#pragma once

#include <vector>
#include <Render/Key.hh>
#include <glm/glm.hpp>

namespace AGE
{

	class Vertices;
	class Properties;
	class Painter;

	struct InstanciedVertice
	{
		Key<Vertices> vertice;
		std::vector<glm::mat4> transformations;
	};

	struct RenderPainter
	{
		std::vector<Key<Vertices>> vertices;
		std::vector<Properties> properties;
//		std::vector<InstanciedVertice> instanciedVertices;
		Key<Painter> painter;
	};

}