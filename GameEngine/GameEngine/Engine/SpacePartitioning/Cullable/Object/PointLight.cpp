#include <SpacePartitioning/Cullable/Object/PointLight.hh>
#include <Utils/MathematicTools.hh>
#include <glm/gtc/matrix_transform.hpp>

namespace AGE
{

	void PointLight::computeSphereTransform()
	{
		float errorRate = 0.01f;
		glm::vec3 lightRange;
		glm::vec3 equation(attenuation.z, attenuation.y, attenuation.x);

		// if the value of equation.z + equation.y * dist + equation.x * dist * dist == 256
		// then the pixels are not lighted anymore (pxlColor = final_color / attenuation)
		equation.z -= 256;
		if (equation.x == 0) // first degree
		{
			// infinite range if there is no distance attenuation
			assert(equation.y != 0);
			lightRange = glm::vec3(-equation.z / equation.y);
		}
		else // second degree
		{
			float d = Mathematic::secondDegreeDiscriminant(equation);
			glm::vec2 res = Mathematic::resolveSecondDegree(equation, d);
			lightRange = glm::vec3(glm::max(res.x, res.y));
		}
		assert(lightRange.x > 0);
		sphereTransform = glm::scale(transformation, lightRange + lightRange * errorRate);
	}
}