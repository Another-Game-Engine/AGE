#include "Graphic/DRBCameraDrawableList.hpp"

namespace AGE
{
	CameraData::CameraData(CameraData const &cam) :
		activated(cam.activated),
		texture(cam.texture),
		projection(cam.projection),
		pipeline(cam.pipeline)
	{
	}

	CameraData::CameraData()
		: activated(true),
		texture(nullptr),
		projection(1.0f),
		pipeline(0)
	{
	}

	CameraData &CameraData::operator = (CameraData const &cam)
	{
		activated = (cam.activated);
		texture = (cam.texture);
		projection = (cam.projection);
		pipeline = (cam.pipeline);
		return *this;
	}
}