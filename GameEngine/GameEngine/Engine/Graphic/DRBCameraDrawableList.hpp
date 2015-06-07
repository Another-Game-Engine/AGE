#pragma once

#include <list>
#include "Culling\Ouptut\RenderCamera.hh"

namespace AGE
{
	struct BFCCullableObject;

	struct DRBCameraDrawableList
	{
		std::list<BFCCullableObject*> meshs;
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