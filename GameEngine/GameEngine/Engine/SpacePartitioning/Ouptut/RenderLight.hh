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
		std::unordered_map<size_t, RenderPainter> keys;
	};

	struct RenderLightList
	{
		std::vector<RenderLight<PointLight>> pointLight;
		std::vector<RenderLight<SpotLight>> spotLights;
		std::vector<RenderLight<DirectionalLight>> directionalLights;
	};

}