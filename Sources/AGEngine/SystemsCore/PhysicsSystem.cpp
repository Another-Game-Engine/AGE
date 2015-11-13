#include <limits>

#include <SystemsCore/PhysicsSystem.hpp>
#include <Physics/EngineTypeToPluginName.hpp>
#include <Physics/WorldInterface.hpp>
#include <Physics/Fallback/NullPhysics.hpp>
#include <ComponentsCore/RigidBody.hpp>
#include <ComponentsCore/Collider.hpp>
#include <ComponentsCore/PhysicsData.hpp>
#include <SystemsCore/CollisionSystem.hpp>
#include <SystemsCore/TriggerSystem.hpp>
#include <Utils/Profiler.hpp>
#include <Utils/MatrixConversion.hpp>

namespace AGE
{
	// Constructors
	PhysicsSystem::PhysicsSystem(AScene *scene, Physics::EngineType physicsEngineType, AssetsManager *assetManager, bool activateSimulation /* true */)
		: System(scene), PluginManager("CreateInterface", "DestroyInterface"), assetManager(assetManager), entityFilter(scene), _activateSimulation(activateSimulation)
	{
		_name = "PhysicsSystem";
		entityFilter.requireComponent<Private::PhysicsData>();
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
		// Set Dependency to the fallback plugin (physics disabled) --> Needed if a RigidBody is added while no PhysicsSystem exists
		_scene->removeInstance<Physics::PhysicsInterface>();
		physics->shutdown(assetManager);
		if (engineType == Physics::EngineType::Null)
		{
			delete physics;
		}
		physics = nullptr;
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

		if (_activateSimulation)
		{
			RigidBody *rigidBody = nullptr;
			Collider *collider = nullptr;
			Link *link = nullptr;
			for (const Entity &entity : entityFilter.getCollection())
			{
				link = entity.getLinkPtr();
				rigidBody = entity->getComponent<RigidBody>();
				if (rigidBody != nullptr && rigidBody->isKinematic() == false)
				{
					//rigidBody->setPosition(posFromMat4(link->getGlobalTransform()));
					// physx n'a pas l'air d'aimer ca
					//rigidBody->setRotation(glm::quat_cast(link->getGlobalTransform()));
					rigidBody->setPosition(link->getPosition());
					rigidBody->setRotation(link->getOrientation());
				}
				else if (rigidBody == nullptr)
				{
					collider = entity->getComponent<Collider>();
					//collider->setPosition(posFromMat4(link->getGlobalTransform()));
					collider->setPosition(link->getPosition());
					// physx n'a pas l'air d'aimer ca
					//collider->setRotation(glm::quat_cast(link->getGlobalTransform()));
					collider->setRotation(link->getOrientation());
				}
			}
		}
	}

	void PhysicsSystem::mainUpdate(float elapsedTime)
	{
		SCOPE_profile_cpu_function("Physic");

		if (_activateSimulation)
		{
			physics->getWorld()->update(elapsedTime);
		}
	}

	void PhysicsSystem::updateEnd(float elapsedTime)
	{
		SCOPE_profile_cpu_function("Physic");

		if (_activateSimulation)
		{
			RigidBody *rigidBody = nullptr;
			Link *link = nullptr;
			for (const Entity &entity : entityFilter.getCollection())
			{
				link = entity.getLinkPtr();
				rigidBody = entity->getComponent<RigidBody>();
				if (rigidBody != nullptr && rigidBody->isKinematic() == false)
				{
					link->setPosition(rigidBody->getPosition());
					link->setOrientation(rigidBody->getRotation());
				}
			}
		}
	}
}