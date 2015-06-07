#pragma once

#include <vector>
#include <memory>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>

#include <list>

namespace AGE
{
	class Properties;
	class IRenderingPipeline;
	struct RenderLightList;
	struct RenderPipeline;
	struct CameraInfos;
	struct BFCCullableObject;

	class IRender
	{
	public:
		virtual ~IRender() {}
		virtual IRender &render(std::list<BFCCullableObject*> const &meshs, RenderLightList &lights, CameraInfos const &infos) = 0;
	};

}