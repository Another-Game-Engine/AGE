#pragma once

#include "SphereColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
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