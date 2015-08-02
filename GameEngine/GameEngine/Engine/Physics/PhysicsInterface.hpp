#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>

#include "EngineType.hpp"
#include "../Utils/DependenciesInjector.hpp"
#include "../AssetManagement/AssetManager.hh"

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
			bool startup(AssetsManager *assetManager);

			void shutdown(AssetsManager *assetManager);

			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			void destroyWorld(void);

			// Virtual Methods
			virtual EngineType getPluginType(void) const = 0;

		protected:
			// Attributes
			WorldInterface *world = nullptr;

			// Virtual Methods
			virtual bool initialize(void) = 0;

			virtual void finalize(void) = 0;

			virtual WorldInterface *createWorld(void) = 0;
		};
	}
}

#include "PhysicsInterface.inl"