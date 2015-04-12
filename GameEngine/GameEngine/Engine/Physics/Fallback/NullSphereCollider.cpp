#pragma once

#include "NullSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullSphereCollider::NullSphereCollider(WorldInterface *world)
			: SphereColliderInterface(world)
		{
			return;
		}
	}
}