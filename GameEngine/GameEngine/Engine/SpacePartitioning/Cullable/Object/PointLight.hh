#pragma once

#include <Utils/Sphere.hh>
#include <Utils/Frustum.hh>
#include <SpacePartitioning/Cullable/CullableShape.hh>
#include <Components/DirectionalLightComponent.hh>
#include <Components/SpotLight.hh>
#include <Components/Light.hh>
#include <glm/glm.hpp>
#include <stdint.h>
#include <Render/Textures/ITexture.hh>
#include <memory>

namespace AGE
{

	struct PointLight : public CullableShape<Sphere>
	{
		// To remove when light will be in octree
		uint32_t activePointLightIdx;
		PointLightData data;
		glm::mat4 sphereTransform;
		void computeSphereTransform();
	};

	struct SpotLight : public CullableShape<Frustum>
	{
		// To remove when light will be in octree
		uint32_t activeSpotLightIdx;
		SpotLightData data;
		void computeDirectionTransform();
	};

	struct DirectionalLight : public Cullable
	{
		uint32_t activeDirectionalLightIdx;
		DirectionalLightData data;
	};

}