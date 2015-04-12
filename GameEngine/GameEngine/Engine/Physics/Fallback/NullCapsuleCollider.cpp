#pragma once

#include "NullCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullCapsuleCollider::NullCapsuleCollider(WorldInterface *world)
			: CapsuleColliderInterface(world)
		{
			return;
		}
	}
}