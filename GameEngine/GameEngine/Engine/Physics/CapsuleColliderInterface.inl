#pragma once

#include "CapsuleColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline CapsuleColliderInterface::CapsuleColliderInterface(WorldInterface *world, void *&data)
			: ColliderInterface(world, data)
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