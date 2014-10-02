#pragma once

#include <glm/fwd.hpp>

namespace Mathematic
{
	float secondDegreeDiscriminant(glm::vec3 const &equation);
	glm::vec2 resolveSecondDegree(glm::vec3 const &equation, float discriminant);
	float resolveSecondDegree(glm::vec3 const &equation);
	float degreeToRadian(float value);
	float radianToDegree(float value);
}