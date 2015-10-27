#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

#include <glm/gtc/matrix_transform.hpp>

//TODO remove when removing properties
#include "Render\Properties\Transformation.hh"

namespace AGE
{
	DRBPointLight::DRBPointLight()
		: BFCCullableObject(BFCCullableType::CullablePointLight)
	{
		_datas = std::make_shared<DRBPointLightData>();
	}

	const std::shared_ptr<DRBPointLightData> DRBPointLight::getDatas() const
	{
		return _datas;
	}

	glm::vec4 DRBPointLight::setBFCTransform(const glm::mat4 &transformation)
	{
		glm::vec4 res = glm::vec4(glm::vec3(transformation[3]), 1.0f);
		_datas->updateRange(glm::translate(glm::mat4(1), glm::vec3(res)));
		// TODO -> remove global property !!!! And datas too !!!
		const std::string str = "model_matrix";
		std::static_pointer_cast<Transformation>(_datas->globalProperties.getProperty(str))->set(transformation);
		return res;
	}
}