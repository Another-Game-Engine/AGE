#pragma once

#include "BulletCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletCapsuleCollider::BulletCapsuleCollider(WorldInterface *world)
			: CapsuleColliderInterface(world)
		{
			return;
		}
	}
}