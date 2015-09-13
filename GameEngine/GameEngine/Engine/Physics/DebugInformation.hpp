#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace AGE
{
	namespace Physics
	{
		struct DebugInformation final
		{
			template <std::uint8_t VerticesNumber>
			struct GenericInformation final
			{
				glm::vec3 position[VerticesNumber];

				float color[VerticesNumber];
			};

			using Point = GenericInformation<1>;
			
			using Line = GenericInformation<2>;

			using Triangle = GenericInformation<3>;

			// Attribute
			std::vector<Point> points;

			std::vector<Line> lines;

			std::vector<Triangle> triangles;
		};
	}
}