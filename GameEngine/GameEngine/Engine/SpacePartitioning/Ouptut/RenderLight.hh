#pragma once

#include <vector>
#include <Render/Textures/Texture2D.hh>
#include <SpacePartitioning/Cullable/Object/PointLight.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <glm/glm.hpp>

namespace AGE
{

	template<typename type_t>
	struct RenderLight
	{
		type_t light;
		std::shared_ptr<ITexture> shadow_map;
		glm::mat4 shadow_matrix;
		std::unordered_map<size_t, RenderPainter> keys;
	};

	struct RenderLightList
	{
		std::vector<RenderLight<PointLight>> pointLight;
		std::vector<RenderLight<SpotLight>> spotLights;
		std::vector<RenderLight<DirectionalLight>> directionalLights;
	};

}