#pragma once

#include "BoxColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		inline glm::vec3 BoxColliderInterface::GetDefaultBoxSize(void)
		{
			return glm::vec3(1.0f, 1.0f, 1.0f);
		}

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