#pragma once

#include <vector>
#include <Render/Key.hh>
#include <glm/glm.hpp>
#include <Core/CullableObjects.hh>

namespace AGE
{

	class Vertices;
	class Painter;

	struct RenderPainter
	{
		std::vector<Key<Vertices>> vertices;
		Key<Painter> painter;
	};

	template<typename type_t>
	struct RenderLight
	{
		type_t light;
		std::vector<RenderPainter> painters;
	};

	struct RenderLightList
	{
		std::vector<RenderLight<PointLight>> pointLight;
	};

	struct RenderPipeline
	{
		std::vector<RenderPainter> painters;
	};

	struct CameraInfos
	{
		glm::mat4 view;
		glm::mat4 projection;
	};

	struct RenderCamera
	{
		CameraInfos camInfos;
		RenderLightList lights;
		std::vector<RenderPipeline> pipelines;
	};

}
