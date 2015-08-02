#pragma once

#include <list>
#include "Culling\Ouptut\RenderCamera.hh"

namespace AGE
{
	struct DRBData;

	struct DRBSpotLightDrawableList
	{
		std::list<std::shared_ptr<DRBData>> meshs;
		std::shared_ptr<DRBData> spotLight;
	};

	struct DRBCameraDrawableList
	{
		std::list<std::shared_ptr<DRBData>> meshs;
		std::list<std::shared_ptr<DRBData>> pointLights;
		std::list<std::shared_ptr<DRBSpotLightDrawableList>> spotLights;
		CameraInfos cameraInfos;
	};
}