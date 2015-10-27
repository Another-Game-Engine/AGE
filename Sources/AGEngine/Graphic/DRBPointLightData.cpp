#include "DRBPointLightData.hpp"

#include "Utils/MathematicTools.hh"

#include <glm/gtc/matrix_transform.hpp>

//TODO remove when removing properties
#include "Render\Properties\Transformation.hh"

namespace AGE
{
	DRBPointLightData::DRBPointLightData()
		: DRBData()
	{
		_rangeProperty = std::make_shared<AutoProperty<glm::vec4, Vec4>>("attenuation_light");
		_positionLightProperty = std::make_shared<AutoProperty<glm::vec3, Vec3>>("position_light");
		std::shared_ptr<Transformation> transformationProperty = std::make_shared<Transformation>(glm::mat4(1));


		globalProperties.add_property(_rangeProperty);
		globalProperties.add_property(_positionLightProperty);
		globalProperties.add_property(transformationProperty);
	}

	DRBPointLightData::~DRBPointLightData()
	{
	}

	void DRBPointLightData::setRange(const glm::vec4 &range)
	{
		_rangeProperty->autoSet(range);
		_range = range;
	}

	void DRBPointLightData::updateRange(const glm::mat4 &transformation)
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
		_positionLightProperty->autoSet(glm::vec3(_sphereTransform[3]));
		//setRange(glm::vec4(lightRange.x, lightRange.y, lightRange.z, 1.0f));
		setTransformation(_sphereTransform);
	}
}