#pragma once

#include "MaterialInterface.hpp"
#include "RigidBodyInterface.hpp"
#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline MaterialInterface::MaterialInterface(ColliderInterface *collider)
			: collider(collider)
		{
			return;
		}

		// Methods
		inline ColliderInterface *MaterialInterface::getCollider(void)
		{
			return collider;
		}

		inline const ColliderInterface *MaterialInterface::getCollider(void) const
		{
			return collider;
		}
	}
}