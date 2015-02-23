#pragma once

#include <vector>
#include <string>
#include <memory>

# define ARGS_FUNCTION_RENDER RenderPipeline &pipeline, RenderLightList &lights, CameraInfos &infos

namespace AGE
{

	class IRendering;
	class Painter;
	class Program;
	class PropertyManager;
	struct RenderPipeline;
	struct RenderLightList;
	struct CameraInfos;

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