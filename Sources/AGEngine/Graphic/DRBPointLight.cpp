#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

#include <glm/gtc/matrix_transform.hpp>

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
		glm::vec4 res = glm::vec4(glm::vec3(transformation[3]), 1.0f);
		_datas->updateRange(glm::translate(glm::mat4(1), glm::vec3(res)));
		return res;
	}

	CullableTypeID DRBPointLight::getBFCType() const
	{
		return BFCCullableType::CullablePointLight;
	}
}