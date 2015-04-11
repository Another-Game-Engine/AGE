#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "EngineTypes.hpp"
#include "WorldInterface.hpp"
#include "../Utils/DependenciesInjector.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysicsInterface : public Dependency<PhysicsInterface>
		{
		public:
			// Constructors
			PhysicsInterface(void) = default;

			PhysicsInterface(const PhysicsInterface &) = delete;

			// Assignment Operators
			PhysicsInterface &operator=(const PhysicsInterface &) = delete;

			// Destructor
			virtual ~PhysicsInterface(void) = default;

			// Methods
			inline bool startup(void)
			{
				return initialize() && createWorld() != nullptr;
			}

			inline void shutdown(void)
			{
				finalize();
				if (world != nullptr)
				{
					destroyWorld();
				}
			}

			inline WorldInterface *createWorld(void)
			{
				assert(world == nullptr && "World already created");
				world = createWorld(glm::vec3(0.0f, -9.81f, 0.0f));
				_dependencyManager->setInstance(world);
				return world;
			}

			inline WorldInterface *getWorld(void)
			{
				assert(world != nullptr && "Invalid world");
				return world;
			}

			inline const WorldInterface *getWorld(void) const
			{
				assert(world != nullptr && "Invalid world");
				return world;
			}

			inline void destroyWorld(void)
			{
				assert(world != nullptr && "Invalid world");
				_dependencyManager->removeInstance<WorldInterface>();
				destroyWorld(world);
				world = nullptr;
			}

			// Virtual Methods
			virtual EngineTypes getPluginType(void) const = 0;

		protected:
			WorldInterface *world = nullptr;

			// Virtual Methods
			virtual bool initialize(void) = 0;

			virtual void finalize(void) = 0;

			virtual WorldInterface *createWorld(const glm::vec3 &gravity) = 0;

			virtual void destroyWorld(WorldInterface *world) = 0;
		};
	}
}