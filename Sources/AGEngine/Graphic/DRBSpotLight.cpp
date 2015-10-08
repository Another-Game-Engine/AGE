#include "DRBSpotLight.hpp"
#include "DRBSpotLightData.hpp"


namespace AGE
{

	DRBSpotLight::DRBSpotLight()
		: BFCCullableObject(BFCCullableType::CullableSpotLight)
	{
		_datas = std::make_shared<DRBSpotLightData>();
	}

	const std::shared_ptr<DRBSpotLightData> DRBSpotLight::getDatas() const
	{
		return _datas;
	}

	glm::vec4 DRBSpotLight::setBFCTransform(const glm::mat4 &transformation)
	{
		_datas->setTransformation(transformation);
		BFCCullableObject::setBFCTransform(transformation);
		_datas->computePositionAndDirection(transformation);
		return glm::vec4(0, 0, 0, 1);
	};
}