#include "BFCCullableObject.hpp"

namespace AGE
{
	glm::vec4 BFCCullableObject::setBFCTransform(const glm::mat4 &transformation)
	{
		_transform = transformation;

		glm::vec3 scale = scaleFromMat4(transformation);
		glm::vec3 translation = posFromMat4(transformation);
		return glm::vec4(translation.x, translation.y, translation.z, glm::compMax(scale));
	};
}