#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "EngineTypes.hpp"
#include "../Utils/DependenciesInjector.hpp"

namespace AGE
{
	namespace Physics
	{
		class WorldInterface;

		class PhysicsInterface : public Dependency < PhysicsInterface >
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
			bool startup(void);

			void shutdown(void);

			WorldInterface *createWorld(void);

			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			void destroyWorld(void);

			// Virtual Methods
			virtual EngineTypes getPluginType(void) const = 0;

		protected:
			// Attributes
			WorldInterface *world = nullptr;

			// Virtual Methods
			virtual bool initialize(void) = 0;

			virtual void finalize(void) = 0;

			virtual WorldInterface *createWorld(const glm::vec3 &gravity) = 0;

			virtual void destroyWorld(WorldInterface *world) = 0;
		};
	}
}

#include "PhysicsInterface.inl"