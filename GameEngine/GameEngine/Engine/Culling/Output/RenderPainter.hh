#pragma once

#include <Render/Pipelining/Render/RenderModes.hh>
#include <Render/Key.hh>
#include <vector>
#include <bitset>
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

	struct RenderDrawableList
	{
		RenderModeSet renderMode;
		std::vector<Key<Vertices>> vertices;
		std::vector<Properties> properties;
	};

	struct RenderPainter
	{
		std::vector<RenderDrawableList> drawables;
//		std::vector<InstanciedVertice> instanciedVertices;
	};

}