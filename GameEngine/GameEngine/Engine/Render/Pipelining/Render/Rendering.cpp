#include <Render/Pipelining/Render/Rendering.hh>

namespace AGE
{

	Rendering::Rendering(std::function<void(FUNCTION_ARGS)> const &function) :
		ARendering(function)
	{

	}

	Rendering::Rendering(Rendering &&move) :
		ARendering(std::move(move))
	{

	}

	IRendering & Rendering::render(FUNCTION_ARGS)
	{
		_render_function(properties, vertices, painter);
		return (*this);
	}

}