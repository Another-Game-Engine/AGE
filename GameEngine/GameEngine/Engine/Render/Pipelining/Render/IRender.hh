#pragma once

#include <vector>
#include <memory>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>

#include <list>

namespace AGE
{
	struct DRBCameraDrawableList;

	class IRender
	{
	public:
		virtual ~IRender() {}
		virtual IRender &render(const DRBCameraDrawableList &infos) = 0;
	};

}