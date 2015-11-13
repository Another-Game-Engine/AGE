#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

#include "Utils/MathematicTools.hh"

#include <glm/gtc/matrix_transform.hpp>

namespace AGE
{
	DRBPointLight::DRBPointLight()
		: BFCCullableObject(BFCCullableType::CullablePointLight)
	{
	}

	glm::vec4 DRBPointLight::setBFCTransform(const glm::mat4 &transformation)
	{
		glm::vec4 res = glm::vec4(glm::vec3(transformation[3]), 1.0f);
		updateRange(glm::translate(glm::mat4(1), glm::vec3(res)));
		// ici j'essaye de modifier la taille de la bounding sphere
		// pour que le culling soit correct mais ca n'apas l'air de fonctionner
		return glm::vec4(glm::vec3(transformation[3]), _range.x);
		//return res;
	}

	void DRBPointLight::setRange(const glm::vec4 &range)
	{
		_range = glm::vec3(range);
	}

	void  DRBPointLight::updateRange(const glm::mat4 &transformation)
	{
		float errorRate = 0.01f;
		glm::vec3 lightRange;
		glm::vec3 equation(_range.z, _range.y, _range.x);

		// if the value of equation.z + equation.y * dist + equation.x * dist * dist == 256
		// then the pixels are not lighted anymore (pxlColor = final_color / attenuation)
		equation.z -= 256;
		if (equation.x == 0) // first degree
		{
			// infinite range if there is no distance attenuation
			AGE_ASSERT(equation.y != 0);
			lightRange = glm::vec3(-equation.z / equation.y);
		}
		else // second degree
		{
			float d = Mathematic::secondDegreeDiscriminant(equation);
			glm::vec2 res = Mathematic::resolveSecondDegree(equation, d);
			lightRange = glm::vec3(glm::max(res.x, res.y));
		}
		AGE_ASSERT(lightRange.x > 0);
		_sphereTransform = glm::scale(transformation, lightRange + lightRange * errorRate);
		_position  = glm::vec3(_sphereTransform[3]);
		_transform = transformation;
	}
}