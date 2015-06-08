#pragma once

#include <vector>
#include <string>
#include <memory>
#include <list>

namespace AGE
{

	class IRender;
	class Painter;
	class Program;
	class PropertyManager;
	struct RenderPipeline;
	struct RenderLightList;
	struct CameraInfos;
	struct BFCCullableObject;
	struct DRBData;

	class IRenderingPipeline
	{
	public:
		virtual ~IRenderingPipeline() {}
		virtual std::string const &name() const = 0;
		virtual IRenderingPipeline &render(std::list<std::shared_ptr<DRBData>> const &meshs, RenderLightList &lights, CameraInfos const &camera) = 0;
		virtual bool recompileShaders() = 0;
	};

}