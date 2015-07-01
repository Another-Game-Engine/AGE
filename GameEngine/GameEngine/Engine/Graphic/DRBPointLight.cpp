#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

namespace AGE
{
	DRBPointLight::DRBPointLight()
	{
		_datas = std::make_shared<DRBPointLightData>();
	}

	const std::shared_ptr<DRBData> DRBPointLight::getDatas() const
	{
		return std::static_pointer_cast<DRBData>(_datas);
	}

	glm::vec4 DRBPointLight::setBFCTransform(const glm::mat4 &transformation)
	{
		auto res = BFCCullableObject::setBFCTransform(transformation);
		_datas->updateRange();
		return res;
	}
}