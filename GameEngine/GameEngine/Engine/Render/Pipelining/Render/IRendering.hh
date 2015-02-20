#pragma once

#include <vector>
#include <memory>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>

namespace AGE
{
	class Properties;
	class IRenderingPipeline;

# define FUNCTION_ARGS std::vector<Key<Properties>> const &properties, std::vector<Key<Vertices>> const &vertices, Painter &painter

	class IRendering
	{
	public:
		virtual ~IRendering() {}
		virtual IRendering &render(FUNCTION_ARGS) = 0;
	};

}