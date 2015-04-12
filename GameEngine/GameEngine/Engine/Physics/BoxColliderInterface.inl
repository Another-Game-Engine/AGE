#pragma once

#include "BoxColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline BoxColliderInterface::BoxColliderInterface(WorldInterface *world)
			: ColliderInterface(world)
		{
			return;
		}

		// Inherited Methods
		inline ColliderType BoxColliderInterface::getColliderType(void) const
		{
			return ColliderType::Box;
		}
	}
}