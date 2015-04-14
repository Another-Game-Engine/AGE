#pragma once

#include "BoxColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline BoxColliderInterface::BoxColliderInterface(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data)
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