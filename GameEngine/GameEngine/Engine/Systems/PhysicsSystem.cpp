#include <Systems/PhysicsSystem.hpp>
#include <Physics/EngineTypeToPluginName.hpp>
#include <Physics/WorldInterface.hpp>
#include <Physics/Fallback/NullPlugin.hpp>

namespace AGE
{
	// Constructors
	PhysicsSystem::PhysicsSystem(AScene *scene, Physics::EngineTypes physicsEngineType)
		: System(scene), PluginManager("CreateInterface", "DestroyInterface")
	{
		_name = "PhysicsSystem";
		if (physicsEngineType == Physics::EngineTypes::Null)
		{
			plugin = new Physics::NullPlugin;
		}
		else if (!addPlugin(Physics::GetPluginNameForEngine(physicsEngineType)))
		{
			AGE_ERROR("Impossible to load physics plugin '", Physics::GetPluginNameForEngine(physicsEngineType), "'. Falling back to physics plugin '", Physics::GetPluginNameForEngine(Physics::EngineTypes::Null), "'.");
			plugin = new Physics::NullPlugin;
		}
	}

	// Destructor
	PhysicsSystem::~PhysicsSystem(void)
	{
		if (plugin != nullptr)
		{
			finalize();
		}
	}

	// Methods
	Physics::PluginInterface *PhysicsSystem::getPlugin(void)
	{
		return plugin;
	}

	// Virtual Methods
	bool PhysicsSystem::initialize(void)
	{
		// Set Dependency to real plugin (physics enabled if plugin != NullPlugin)
		_scene->deleteInstance<Physics::PluginInterface>();
		_scene->setInstance(plugin);
		Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Initializing PhysicsSystem with plugin '", Physics::GetPluginNameForEngine(plugin->getPluginType()), "'.");
		return plugin->startup();
	}

	void PhysicsSystem::finalize(void)
	{
		assert(plugin != nullptr && "System already finalized");
		Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Finalizing PhysicsSystem with plugin '", Physics::GetPluginNameForEngine(plugin->getPluginType()), "'.");
		plugin->shutdown();
		plugin = nullptr;
		// Set Dependency to the fallback plugin (physics disabled) --> Needed if a RigidBody is added while no PhysicsSystem exists
		_scene->removeInstance<Physics::PluginInterface>();
		_scene->setInstance<Physics::NullPlugin, Physics::PluginInterface>();
	}

	bool PhysicsSystem::onPluginLoaded(PluginPtr pluginData)
	{
		plugin = pluginData->plugin;
		return true;
	}
}