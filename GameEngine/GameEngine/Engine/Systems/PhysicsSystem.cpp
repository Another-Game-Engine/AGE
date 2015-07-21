#include <limits>

#include <Systems/PhysicsSystem.hpp>
#include <Physics/EngineTypeToPluginName.hpp>
#include <Physics/WorldInterface.hpp>
#include <Physics/Fallback/NullPhysics.hpp>
#include <Components/RigidBody.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/TriggerSystem.hpp>
#include <Utils/Profiler.hpp>

namespace AGE
{
	// Constructors
	PhysicsSystem::PhysicsSystem(AScene *scene, Physics::EngineType physicsEngineType, AssetsManager *assetManager)
		: System(scene), PluginManager("CreateInterface", "DestroyInterface"), assetManager(assetManager), entityFilter(scene)
	{
		_name = "PhysicsSystem";
		entityFilter.requireComponent<RigidBody>();
		if (physicsEngineType == Physics::EngineType::Null)
		{
			physics = new Physics::NullPhysics;
		}
		else if (!addPlugin(Physics::GetPluginNameForEngine(physicsEngineType)))
		{
			AGE_ERROR("Impossible to load physics plugin '", Physics::GetPluginNameForEngine(physicsEngineType), "'. Falling back to physics plugin '", Physics::GetPluginNameForEngine(Physics::EngineType::Null), "'.");
			physics = new Physics::NullPhysics;
		}
	}

	// Destructor
	PhysicsSystem::~PhysicsSystem(void)
	{
		if (physics != nullptr)
		{
			finalize();
		}
	}

	// Methods
	Physics::PhysicsInterface *PhysicsSystem::getPhysics(void)
	{
		return physics;
	}

	const Physics::PhysicsInterface *PhysicsSystem::getPhysics(void) const
	{
		return physics;
	}

	// Inherited Methods
	bool PhysicsSystem::initialize(void)
	{
		// Set Dependency to real plugin (physics enabled if plugin != NullPhysics)
		if (_scene->hasInstance<Physics::PhysicsInterface>())
		{
			_scene->getInstance<Physics::PhysicsInterface>()->shutdown(assetManager);
			_scene->deleteInstance<Physics::PhysicsInterface>();
		}
		_scene->setInstance(physics);
		Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Initializing PhysicsSystem with plugin '", Physics::GetPluginNameForEngine(physics->getPluginType()), "'.");
		if (physics->startup(assetManager))
		{
			_scene->addSystem<Private::CollisionSystem>(std::numeric_limits<std::size_t>::min());
			_scene->addSystem<Private::TriggerSystem>(std::numeric_limits<std::size_t>::min());
			return true;
		}
		else
		{
			return false;
		}
	}

	void PhysicsSystem::finalize(void)
	{
		assert(physics != nullptr && "System already finalized");
		Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Finalizing PhysicsSystem with plugin '", Physics::GetPluginNameForEngine(physics->getPluginType()), "'.");
		const Physics::EngineType engineType = physics->getPluginType();
		physics->shutdown(assetManager);
		if (engineType == Physics::EngineType::Null)
		{
			delete physics;
		}
		physics = nullptr;
		// Set Dependency to the fallback plugin (physics disabled) --> Needed if a RigidBody is added while no PhysicsSystem exists
		_scene->removeInstance<Physics::PhysicsInterface>();
		_scene->setInstance<Physics::NullPhysics, Physics::PhysicsInterface>()->startup(assetManager);
	}

	bool PhysicsSystem::onPluginLoaded(PluginPtr pluginData)
	{
		physics = pluginData->plugin;
		return true;
	}

	void PhysicsSystem::updateBegin(float elapsedTime)
	{
		SCOPE_profile_cpu_function("Physic");

		RigidBody *rigidBody = nullptr;
		Link *link = nullptr;
		for (Entity entity : entityFilter.getCollection())
		{
			link = entity.getLinkPtr();
			rigidBody = entity->getComponent<RigidBody>();
			rigidBody->setPosition(link->getPosition());
			rigidBody->setRotation(link->getOrientation());
		}
	}

	void PhysicsSystem::mainUpdate(float elapsedTime)
	{
		SCOPE_profile_cpu_function("Physic");

		physics->getWorld()->update(elapsedTime);
	}

	void PhysicsSystem::updateEnd(float elapsedTime)
	{
		SCOPE_profile_cpu_function("Physic");

		RigidBody *rigidBody = nullptr;
		Link *link = nullptr;
		for (Entity entity : entityFilter.getCollection())
		{
			rigidBody = entity->getComponent<RigidBody>();
			link = entity.getLinkPtr();
			link->setPosition(rigidBody->getPosition(), false);
			link->setOrientation(rigidBody->getRotation());
		}
	}
}