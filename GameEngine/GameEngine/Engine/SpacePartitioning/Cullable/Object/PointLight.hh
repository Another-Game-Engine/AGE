#pragma once

#include <Utils/Sphere.hh>
#include <SpacePartitioning/Cullable/CullableShape.hh>
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

		std::shared_ptr<ITexture> map;
		glm::vec3 attenuation;
		glm::vec3 color;
		glm::mat4 sphereTransform;

		PointLight();
		PointLight(glm::vec3 const &position, glm::vec3 const &color, glm::vec3 const &attenuation);

		void computeSphereTransform();
	};

}