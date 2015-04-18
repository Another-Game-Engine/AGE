#pragma once

#include "MaterialInterface.hpp"
#include "RigidBodyInterface.hpp"
#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		inline float MaterialInterface::GetDefaultStaticFriction(void)
		{
			return 0.5f;
		}

		inline float MaterialInterface::GetDefaultDynamicFriction(void)
		{
			return 0.5f;
		}

		inline float MaterialInterface::GetDefaultRestitution(void)
		{
			return 0.1f;
		}

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