#include <Render/Pipelining/Render/Rendering.hh>

Rendering::Rendering(std::function<IRendering &(FUNCTION_ARGS)> const &function):
ARendering(function)
{

}

Rendering::Rendering(Rendering &&move) :
ARendering(std::move(move))
{

}

IRendering & Rendering::render(std::vector<AGE::Drawable> const &drawables, IRenderingPipeline &_, PaintingManager &paintingManager, std::vector<std::shared_ptr<IRenderingPipeline>> const &other)
{
	_render_function(drawables, _, paintingManager, other);
	return (*this);
}

