#pragma once

#include "MaterialInterface.hpp"
#include "RigidBodyInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		inline RigidBodyInterface *MaterialInterface::getRigidBody(void)
		{
			return rigidBody;
		}

		inline const RigidBodyInterface *MaterialInterface::getRigidBody(void) const
		{
			return rigidBody;
		}
	}
}