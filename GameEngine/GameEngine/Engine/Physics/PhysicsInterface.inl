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
			return initialize() && createWorld() != nullptr;
		}

		inline void PhysicsInterface::shutdown(void)
		{
			if (world != nullptr)
			{
				destroyWorld();
			}
			finalize();
		}

		inline WorldInterface *PhysicsInterface::createWorld(void)
		{
			assert(world == nullptr && "World already created");
			world = createWorld(glm::vec3(0.0f, -9.81f, 0.0f));
			_dependencyManager->setInstance(world);
			return world;
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