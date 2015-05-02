#pragma once

#include <vector>
#include <SpacePartitioning/Cullable/Object/PointLight.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>

namespace AGE
{

	template<typename type_t>
	struct RenderLight
	{
		type_t light;
		std::vector<RenderPainter> painters;
	};

	struct RenderLightList
	{
		std::vector<RenderLight<PointLight>> pointLight;
		std::vector<RenderLight<SpotLight>> spotLights;
	};

}