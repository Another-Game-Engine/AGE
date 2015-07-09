#include "DRBSpotLight.hpp"
#include "DRBSpotLightData.hpp"

namespace AGE
{
	DRBSpotLight::DRBSpotLight()
	{
		_datas = std::make_shared<DRBSpotLightData>();
	}

	const std::shared_ptr<DRBData> DRBSpotLight::getDatas() const
	{
		return std::static_pointer_cast<DRBData>(_datas);
	}

	glm::vec4 DRBSpotLight::setBFCTransform(const glm::mat4 &transformation)
	{
		_datas->setTransformation(transformation);
		BFCCullableObject::setBFCTransform(transformation);
		_datas->computePositionAndDirection(transformation);
		return glm::vec4(0, 0, 0, 1);
	};
}