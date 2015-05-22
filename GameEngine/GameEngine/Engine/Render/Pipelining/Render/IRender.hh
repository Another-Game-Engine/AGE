#pragma once

#include <vector>
#include <memory>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>

namespace AGE
{
	class Properties;
	class IRenderingPipeline;
	struct RenderLightList;
	struct RenderPipeline;
	struct CameraInfos;

	class IRender
	{
	public:
		virtual ~IRender() {}
		virtual IRender &render(RenderPipeline const &pipeline, RenderLightList &lights, CameraInfos const &infos) = 0;
	};

}