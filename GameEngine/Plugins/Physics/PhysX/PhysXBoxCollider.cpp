#pragma once

#include "PhysXBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXBoxCollider::PhysXBoxCollider(WorldInterface *world)
			: BoxColliderInterface(world)
		{
			return;
		}
	}
}