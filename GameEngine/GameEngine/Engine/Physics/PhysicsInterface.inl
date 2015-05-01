#pragma once

#include <cereal/cereal.hpp>

#include "PhysicsInterface.hpp"
#include "WorldInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		inline bool PhysicsInterface::startup(const std::string &assetDirectory)
		{
			if (initialize())
			{
				assert(world == nullptr && "World already created");
				world = createWorld();
				assert(world != nullptr && "Impossible to create world");
				return world->initialize(assetDirectory);
			}
			else
			{
				return false;
			}
		}

		inline void PhysicsInterface::shutdown(const std::string &assetDirectory)
		{
			if (world != nullptr)
			{
				world->finalize(assetDirectory);
				destroyWorld();
			}
			finalize();
		}

		inline WorldInterface *PhysicsInterface::getWorld(void)
		{
			assert(world != nullptr && "Invalid world");
			return world;
		}

		inline const WorldInterface *PhysicsInterface::getWorld(void) const
		{
			assert(world != nullptr && "Invalid world");
			return world;
		}

		inline void PhysicsInterface::destroyWorld(void)
		{
			assert(world != nullptr && "Invalid world");
			delete world;
			world = nullptr;
		}
	}
}