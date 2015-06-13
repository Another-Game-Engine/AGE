#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

namespace AGE
{
	DRBPointLight::DRBPointLight()
	{}

	const std::shared_ptr<DRBData> DRBPointLight::getDatas() const
	{
		return std::static_pointer_cast<DRBData>(_datas);
	}
}