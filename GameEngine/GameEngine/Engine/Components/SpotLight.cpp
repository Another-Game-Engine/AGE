#include <Components\SpotLight.hh>
#include <glm/glm.hpp>


namespace AGE
{
	SpotLightData::SpotLightData() { }

	SpotLightData::~SpotLightData()
	{
	}


	SpotLight::SpotLight()
	{
	}

	SpotLight::~SpotLight()
	{
	}

	void		SpotLight::updateLightData(const glm::mat4 &globalTransform)
	{
		lightData.lightVP = projection * glm::inverse(globalTransform);
		lightData.positionPower.x = globalTransform[3].x;
		lightData.positionPower.y = globalTransform[3].y;
		lightData.positionPower.z = globalTransform[3].z;
	}

	void SpotLight::reset()
	{
	}

	void	SpotLight::init()
	{
	}

	template <typename Archive>
	void SpotLight::serialize(Archive &ar)
	{
	}

}
