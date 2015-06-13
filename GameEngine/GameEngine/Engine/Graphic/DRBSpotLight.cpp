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
}