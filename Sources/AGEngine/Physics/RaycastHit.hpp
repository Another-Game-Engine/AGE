#pragma once

#include "Entity/Entity.hh"

namespace AGE
{
	namespace Physics
	{
		struct RaycastHit final
		{
			// Attributes
			Entity hitEntity;

			float distanceFromRayOrigin;

			glm::vec3 impactPoint;

			glm::vec3 impactNormal;
		};
	}
}