#pragma once

#include "BulletSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletSphereCollider::BulletSphereCollider(WorldInterface *world)
			: SphereColliderInterface(world)
		{
			return;
		}
	}
}