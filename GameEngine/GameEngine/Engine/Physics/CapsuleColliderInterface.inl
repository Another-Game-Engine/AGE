#pragma once

#include "CapsuleColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline CapsuleColliderInterface::CapsuleColliderInterface(WorldInterface *world)
			: ColliderInterface(world)
		{
			return;
		}

		// Inherited Methods
		inline ColliderType CapsuleColliderInterface::getColliderType(void) const
		{
			return ColliderType::Capsule;
		}
	}
}