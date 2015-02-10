#include <Components\SpotLight.hh>
#include <glm/glm.hpp>

namespace AGE
{

	SpotLightData::SpotLightData() { }

	SpotLightData::SpotLightData(SpotLightData const &o)
	{
		*this = o;
	}

	SpotLightData::~SpotLightData()
	{
	}

	SpotLightData &SpotLightData::operator = (SpotLightData const &o)
	{
		lightVP = o.lightVP;
		positionPower = o.positionPower;
		colorRange = o.colorRange;
		shadowId = o.shadowId;
		return (*this);
	}


	SpotLight::SpotLight()
	{
	}

	SpotLight::SpotLight(SpotLight const &o)
	{
		*this = o;
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

	void SpotLight::reset(AScene *)
	{
	}

	SpotLight &SpotLight::operator=(SpotLight const &o)
	{
		lightData = o.lightData;
		return (*this);
	}

	void	SpotLight::init(AScene *)
	{
	}

	template <typename Archive>
	void SpotLight::serialize(Archive &ar)
	{
	}

}
