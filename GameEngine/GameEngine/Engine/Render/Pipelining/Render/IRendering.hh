#pragma once

# include <vector>
# include <memory>
# include <Render/GeometryManagement/Painting/PaintingManager.hh>
# include <Core/CullableObjects.hh>

namespace AGE
{
	class IRenderingPipeline;

# define FUNCTION_ARGS std::vector<Key<Vertices>> const &vertices, Painter &painter

	class IRendering
	{
	public:
		virtual ~IRendering() {}
		virtual IRendering &render(FUNCTION_ARGS) = 0;
	};

}