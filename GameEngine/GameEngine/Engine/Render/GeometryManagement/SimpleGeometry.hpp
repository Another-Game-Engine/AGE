#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>

namespace AGE
{
	namespace Quad
	{
		static const std::size_t PositionsNumber = 4;
		static const std::vector<glm::vec4> Positions = // position x,y - UVs z,w
		{
			glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f)
			, glm::vec4(-1.0f, -1.0f, 0.0f, 0.0f)
			, glm::vec4(1.0f, -1.0f, 1.0f, 0.0f)
			, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		static const std::size_t IndicesNumber = 6;
		static const std::vector<unsigned int> Indices =
		{
			0, 1, 2, 0, 3, 2
		};
		static AGE::Key<AGE::Vertices> VerticesKey;
		static AGE::Key<AGE::Painter> PainterKey;
	}
}