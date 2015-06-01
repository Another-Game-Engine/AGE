#pragma once

#include <SpacePartitioning/Cullable/CullableShape.hh>
#include <Utils/Frustum.hh>
#include <glm/glm.hpp>
#include <Components/CameraComponent.hpp>

namespace AGE
{

	struct Camera : public CullableShape<Frustum>
	{
		uint32_t activeCameraIdx;
		CameraData data;
	};

}