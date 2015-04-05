#include <Systems/PhysicsSystem.hpp>
#include <Physics/EngineTypeToPluginName.hpp>

namespace AGE
{
	PhysicsSystem::PhysicsSystem(AScene *scene, Physics::EngineTypes physicsEngineType)
		: System(scene), PluginManager("CreateInterface", "DestroyInterface")
	{
		_name = "PhysicsSystem";
		if (!addPlugin(Physics::GetPluginNameForEngine(physicsEngineType)))
		{
			AGE_ERROR("Impossible to load physics plugin '", Physics::GetPluginNameForEngine(physicsEngineType), "'. Falling back to physics plugin '", Physics::GetPluginNameForEngine(Physics::EngineTypes::Null), "'.");
			if (!addPlugin(Physics::GetPluginNameForEngine(Physics::EngineTypes::Null)))
			{
				AGE_FATAL("Impossible to load any physics plugin.");
			}
		}
	}

	// Virtual Methods
	bool PhysicsSystem::initialize(void)
	{
		if (plugin)
		{
			Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Initializing PhysicsSystem with plugin '", plugin->getPluginName(), "'.");
			return true;
		}
		else
		{
			return false;
		}
	}

	bool PhysicsSystem::onPluginLoaded(PluginPtr pluginData)
	{
		plugin = pluginData->plugin;
		return true;
	}
}