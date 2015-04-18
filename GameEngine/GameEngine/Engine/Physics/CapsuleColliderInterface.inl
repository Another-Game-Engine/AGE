#pragma once

#include "CapsuleColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		inline float CapsuleColliderInterface::GetDefaultHeight(void)
		{
			return 1.0f;
		}

		inline float CapsuleColliderInterface::GetDefaultRadius(void)
		{
			return 0.5f;
		}

		// Constructors
		inline CapsuleColliderInterface::CapsuleColliderInterface(WorldInterface *world, Private::GenericData *data)
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