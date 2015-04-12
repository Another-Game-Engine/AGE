#pragma once

#include "SphereColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline SphereColliderInterface::SphereColliderInterface(WorldInterface *world)
			: ColliderInterface(world)
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