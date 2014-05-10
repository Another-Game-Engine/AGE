
#include <Components\SpotLight.hh>

namespace Component
{

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

	void SpotLight::reset()
	{
	}

	SpotLight &SpotLight::operator=(SpotLight const &o)
	{
		lightData = o.lightData;
		return (*this);
	}

	void	SpotLight::init()
	{
	}

	template <typename Archive>
	void SpotLight::serialize(Archive &ar)
	{
	}

}