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
		inline MaterialInterface::MaterialInterface(const std::string &name)
			: name(name)
		{
			return;
		}

		// Methods
		inline const std::string &MaterialInterface::getName(void) const
		{
			return name;
		}
	}
}