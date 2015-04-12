#pragma once

#include "BulletBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletBoxCollider::BulletBoxCollider(WorldInterface *world)
			: BoxColliderInterface(world)
		{
			return;
		}
	}
}