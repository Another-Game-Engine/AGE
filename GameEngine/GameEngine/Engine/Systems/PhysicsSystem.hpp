#pragma once

#include <Systems/System.h>

#include <Utils/Library.hpp>

#include <Physics/EngineTypes.hpp>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>

namespace AGE
{
	class PhysicsSystem final : public System, public PluginManager<Physics::PhysicsInterface>
	{
	public:
		// Constructors
		PhysicsSystem(void) = delete;

		PhysicsSystem(AScene *scene, Physics::EngineTypes physicsEngineType);

		PhysicsSystem(PhysicsSystem const &) = delete;

		// Assignment Operators
		PhysicsSystem &operator=(PhysicsSystem const &) = delete;

		// Destructors
		~PhysicsSystem(void);

		// Methods
		Physics::PhysicsInterface *getPlugin(void);

	private:
		// Attributes
		Physics::PhysicsInterface *plugin = nullptr;

		// Inherited Methods
		bool initialize(void) override final;

		void finalize(void) override final;

		bool onPluginLoaded(PluginPtr pluginData) override final;
	};
}