#include <Render/Pipelining/Render/Rendering.hh>

namespace AGE
{

	Rendering::Rendering(std::function<void(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)> const &function) :
		ARendering(function)
	{

	}

	Rendering::Rendering(Rendering &&move) :
		ARendering(std::move(move))
	{

	}

	IRendering & Rendering::render(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)
	{
		_render_function(properties, vertices, painter);
		return (*this);
	}

}