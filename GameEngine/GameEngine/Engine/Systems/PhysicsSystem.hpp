#pragma once

#include <Systems/System.h>

#include <Utils/Library.hpp>

#include <Physics/EngineType.hpp>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>

#include <AssetManagement/AssetManager.hh>

namespace AGE
{
	class PhysicsSystem final : public System, public PluginManager<Physics::PhysicsInterface>
	{
	public:
		// Constructors
		PhysicsSystem(void) = delete;

		PhysicsSystem(AScene *scene, Physics::EngineType physicsEngineType, AssetsManager *assetManager, bool activateSimulation = true);

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
		AssetsManager *assetManager;

		Physics::PhysicsInterface *physics = nullptr;

		EntityFilter entityFilter;

		const bool _activateSimulation;

		// Inherited Methods
		bool initialize(void) override final;

		void finalize(void) override final;

		bool onPluginLoaded(PluginPtr pluginData) override final;

		void updateBegin(float elapsedTime) override final;

		void mainUpdate(float elapsedTime) override final;

		void updateEnd(float elapsedTime) override final;
	};
}