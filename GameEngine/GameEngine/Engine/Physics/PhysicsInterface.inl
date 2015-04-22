#pragma once

#include "PhysicsInterface.hpp"
#include "WorldInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		inline bool PhysicsInterface::startup(void)
		{
			if (initialize())
			{
				assert(world == nullptr && "World already created");
				world = createWorld();
				assert(world != nullptr && "Impossible to create world");
				_dependencyManager->setInstance(world);
				return true;
			}
			else
			{
				return false;
			}
		}

		inline void PhysicsInterface::shutdown(void)
		{
			if (world != nullptr)
			{
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
			_dependencyManager->removeInstance<WorldInterface>();
			delete world;
			world = nullptr;
		}
	}
}