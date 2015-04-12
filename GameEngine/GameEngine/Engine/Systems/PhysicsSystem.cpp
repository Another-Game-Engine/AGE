#include <limits>

#include <Systems/PhysicsSystem.hpp>
#include <Physics/EngineTypeToPluginName.hpp>
#include <Physics/WorldInterface.hpp>
#include <Physics/Fallback/NullPhysics.hpp>
#include <Components/NewRigidBody.hpp>
#include <Systems/CollisionSystem.hpp>

namespace AGE
{
	// Constructors
	PhysicsSystem::PhysicsSystem(AScene *scene, Physics::EngineType physicsEngineType)
		: System(scene), PluginManager("CreateInterface", "DestroyInterface"), entityFilter(scene)
	{
		_name = "PhysicsSystem";
		entityFilter.requireComponent<NewRigidBody>();
		if (physicsEngineType == Physics::EngineType::Null)
		{
			physics = new Physics::NullPhysics;
		}
		else if (!addPlugin(Physics::GetPluginNameForEngine(physicsEngineType)))
		{
			AGE_ERROR("Impossible to load physics plugin '", Physics::GetPluginNameForEngine(physicsEngineType), "'. Falling back to physics plugin '", Physics::GetPluginNameForEngine(Physics::EngineType::Null), "'.");
			physics = new Physics::NullPhysics;
		}
		scene->addSystem<Private::CollisionSystem>(std::numeric_limits<std::size_t>::max(), physics);
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
		_scene->getInstance<Physics::PhysicsInterface>()->shutdown();
		_scene->deleteInstance<Physics::PhysicsInterface>();
		_scene->setInstance(physics);
		Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Initializing PhysicsSystem with plugin '", Physics::GetPluginNameForEngine(physics->getPluginType()), "'.");
		return physics->startup();
	}

	void PhysicsSystem::finalize(void)
	{
		assert(physics != nullptr && "System already finalized");
		Singleton<Logger>::getInstance()->log(Logger::Level::Normal, "Finalizing PhysicsSystem with plugin '", Physics::GetPluginNameForEngine(physics->getPluginType()), "'.");
		physics->shutdown();
		physics = nullptr;
		// Set Dependency to the fallback plugin (physics disabled) --> Needed if a RigidBody is added while no PhysicsSystem exists
		_scene->removeInstance<Physics::PhysicsInterface>();
		_scene->setInstance<Physics::NullPhysics, Physics::PhysicsInterface>()->startup();
	}

	bool PhysicsSystem::onPluginLoaded(PluginPtr pluginData)
	{
		physics = pluginData->plugin;
		return true;
	}

	void PhysicsSystem::mainUpdate(float elapsedTime)
	{
		for (Entity entity : entityFilter.getCollection())
		{
			NewRigidBody *rigidBody = entity.getComponent<NewRigidBody>();
			Link *link = entity.getLinkPtr();
			rigidBody->setPosition(link->getPosition());
			rigidBody->setRotation(link->getOrientation());
		}
		physics->getWorld()->update(elapsedTime);
		for (Entity entity : entityFilter.getCollection())
		{
			NewRigidBody *rigidBody = entity.getComponent<NewRigidBody>();
			Link *link = entity.getLinkPtr();
			link->setPosition(rigidBody->getPosition());
			link->setOrientation(rigidBody->getRotation());
		}
	}
}