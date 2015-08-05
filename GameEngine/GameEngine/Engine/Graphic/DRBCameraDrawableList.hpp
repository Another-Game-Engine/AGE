#pragma once

#include <list>
#include "Culling\Output\RenderCamera.hh"

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

	struct DRBCameraDrawableListCommand
	{
		DRBCameraDrawableListCommand(std::shared_ptr<DRBCameraDrawableList> _list)
			: list(_list)
		{}
		DRBCameraDrawableListCommand(DRBCameraDrawableListCommand &&o)
		{
			list = std::move(o.list);
		}
		std::shared_ptr<DRBCameraDrawableList> list;
	};
}