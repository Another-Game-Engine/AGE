#pragma once

# include <Render/Pipelining/Render/IRendering.hh>
# include <functional>

namespace AGE
{

	class ARendering : public IRendering
	{
	public:
		virtual ~ARendering() {}

	protected:
		ARendering(std::function<void(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)> const & function);
		ARendering(ARendering &&move);

	protected:
		std::function<void(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)> _render_function;
	};

}