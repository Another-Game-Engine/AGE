#pragma once

#include "SphereColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		inline glm::vec3 SphereColliderInterface::GetDefaultCenter(void)
		{
			return glm::vec3();
		}

		inline float SphereColliderInterface::GetDefaultRadius(void)
		{
			return 1.0f;
		}

		// Constructors
		inline SphereColliderInterface::SphereColliderInterface(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data)
		{
			return;
		}

		// Inherited Methods
		inline ColliderType SphereColliderInterface::getColliderType(void) const
		{
			return ColliderType::Sphere;
		}
	}
}