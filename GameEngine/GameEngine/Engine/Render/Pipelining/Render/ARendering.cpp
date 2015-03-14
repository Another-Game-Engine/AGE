#include <Render/Pipelining/Render/ARendering.hh>

namespace AGE
{

	ARendering::ARendering(std::function<void(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)> const &function) :
		_render_function(function)
	{

	}

	ARendering::ARendering(ARendering &&move) :
		_render_function(std::move(move._render_function))
	{

	}

}