#include "DRBSpotLight.hpp"

namespace AGE
{

	DRBSpotLight::DRBSpotLight()
		: BFCCullableObject(BFCCullableType::CullableSpotLight)
	{
	}

	glm::vec4 DRBSpotLight::setBFCTransform(const glm::mat4 &transformation)
	{
		BFCCullableObject::setBFCTransform(transformation);
		return glm::vec4(0, 0, 0, 1);
	};
}