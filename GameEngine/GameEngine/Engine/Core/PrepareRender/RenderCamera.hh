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

	struct RenderLightList
	{
		RenderLight<PointLight> _pointLight;
	};

	template<typename type_t>
	struct RenderLight
	{
		type_t light;
		std::vector<RenderPainter> painters;
	};

	struct RenderPipeline
	{
		std::vector<RenderPainter> painters;
	};

	struct RenderCamera
	{
		glm::mat4 view;
		glm::mat4 projection;
		std::vector<RenderLight<PointLight>> pointLights;
		std::vector<RenderPipeline> pipelines;
	};

}
