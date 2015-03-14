#pragma once

# include <Render/Pipelining/Render/ARendering.hh>

namespace AGE
{

	class Rendering : public ARendering
	{
	public:
		Rendering(std::function<void(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)> const &function);
		Rendering(Rendering &&move);
		virtual ~Rendering() {}

	public:
		virtual IRendering &render(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter) override final;
	};
}