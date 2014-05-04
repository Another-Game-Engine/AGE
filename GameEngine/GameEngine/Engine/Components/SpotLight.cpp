
#include <Components\SpotLight.hh>
#include <Entities\EntityData.hh>

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

	void		SpotLight::updateLightData()
	{
		//@CESAR TODO
		//lightData.lightVP = projection * glm::inverse(_entity->getGlobalTransform());
		//lightData.positionPower.x = _entity->getGlobalTransform()[3].x;
		//lightData.positionPower.y = _entity->getGlobalTransform()[3].y;
		//lightData.positionPower.z = _entity->getGlobalTransform()[3].z;
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