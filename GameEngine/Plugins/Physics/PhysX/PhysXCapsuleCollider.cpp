#pragma once

#include "PhysXCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXCapsuleCollider::PhysXCapsuleCollider(WorldInterface *world)
			: CapsuleColliderInterface(world)
		{
			return;
		}
	}
}