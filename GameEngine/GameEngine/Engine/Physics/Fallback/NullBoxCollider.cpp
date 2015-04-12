#pragma once

#include "NullBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullBoxCollider::NullBoxCollider(WorldInterface *world)
			: BoxColliderInterface(world)
		{
			return;
		}
	}
}