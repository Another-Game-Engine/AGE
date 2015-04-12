#pragma once

#include "PhysXSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXSphereCollider::PhysXSphereCollider(WorldInterface *world)
			: SphereColliderInterface(world)
		{
			return;
		}
	}
}