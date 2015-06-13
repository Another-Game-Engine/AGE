#pragma once

#include <list>
#include "Culling\Ouptut\RenderCamera.hh"

namespace AGE
{
	struct DRBData;

	struct DRBCameraDrawableList
	{
		std::list<std::shared_ptr<DRBData>> meshs;
		std::list<std::shared_ptr<DRBData>> pointLights;
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