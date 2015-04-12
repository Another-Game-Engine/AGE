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
		Physics::PhysicsInterface *getPhysics(void);

		const Physics::PhysicsInterface *getPhysics(void) const;

	private:
		// Attributes
		Physics::PhysicsInterface *physics = nullptr;

		EntityFilter entityFilter;

		// Inherited Methods
		bool initialize(void) override final;

		void finalize(void) override final;

		bool onPluginLoaded(PluginPtr pluginData) override final;

		void mainUpdate(float elapsedTime) override final;
	};
}