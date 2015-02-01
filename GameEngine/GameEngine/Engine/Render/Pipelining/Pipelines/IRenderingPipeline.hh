#pragma once

# include <vector>
# include <string>
# include <memory>
# include <Render/GeometryManagement/Painting/PaintingManager.hh>
# include <Core/CullableObjects.hh>
# include <Core/PrepareRender/RenderCamera.hh>

# define ARGS_FUNCTION_RENDER std::vector<Key<Vertices>> &pipeline, RenderLightList &lights

namespace AGE
{

	class IRendering;
	class Painter;
	class Program;

	class IRenderingPipeline
	{
	public:
		virtual ~IRenderingPipeline() {}
		virtual std::vector<std::shared_ptr<IRendering>> const &get_rendering() const = 0;
		virtual std::string const &name() const = 0;
		virtual std::vector<std::shared_ptr<Program>> const &get_programs() const = 0;
		virtual IRenderingPipeline &render(ARGS_FUNCTION_RENDER) = 0;
	};

}