#include <Utils/MathematicTools.hh>
#include <glm/glm.hpp>
#include <math.h>
#include <assert.h>

namespace Mathematic
{
	// equation = ax² + by + z
	float secondDegreeDiscriminant(glm::vec3 const &equation)
	{
		return (pow(equation.y, 2.f) - 4.f * equation.z * equation.x);
	}

	// equation = ax² + by + z
	glm::vec2 resolveSecondDegree(glm::vec3 const &equation, float discriminant)
	{
		glm::vec2 result;

		if (discriminant <= 0)
			assert(0);
		result.x = (-equation.y + sqrt(discriminant)) / (2.f * equation.z);
		result.y = (-equation.y - sqrt(discriminant)) / (2.f * equation.z);
		return (result);
	}

	// equation = az² + by + x
	float resolveSecondDegree(glm::vec3 const &equation)
	{
		return (-equation.y / (2.0f * equation.z));
	}

	// conversion degree
	float degreeToRadian(float value)
	{
		return (3.14159f * (value) / 180.0f);
	}

	float radianToDegree(float value)
	{
		return (180.f * (value) / 3.14159f);
	}
}