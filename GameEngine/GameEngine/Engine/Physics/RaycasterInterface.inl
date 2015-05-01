#pragma once

#include <cassert>

#include "RaycasterInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		inline RaycasterInterface::RaycasterInterface(WorldInterface *world)
			: world(world)
		{
			assert(world != nullptr && "Invalid world");
		}

		// Methods
		inline WorldInterface *RaycasterInterface::getWorld(void)
		{
			return world;
		}

		inline const WorldInterface *RaycasterInterface::getWorld(void) const
		{
			return world;
		}
	}
}