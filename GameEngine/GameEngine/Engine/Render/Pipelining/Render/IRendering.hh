#pragma once

# include <vector>
# include <memory>
# include <Render/GeometryManagement/Painting/PaintingManager.hh>
# include <Core/CullableObjects.hh>

class IRenderingPipeline;

# define FUNCTION_ARGS std::vector<AGE::Drawable> const &drawables, IRenderingPipeline &, PaintingManager &paintingManager, std::vector<std::shared_ptr<IRenderingPipeline>> const &

class IRendering
{
public:
	virtual ~IRendering() {}
	virtual IRendering &render(FUNCTION_ARGS) = 0;
};