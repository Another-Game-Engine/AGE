#include <thread>
#include <cassert>

#include "PhysXWorld.hpp"
#include "PhysXRigidBody.hpp"
#include "PhysXMaterial.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXWorld::PhysXWorld(PhysXPhysics *physics, const glm::vec3 &gravity)
			: WorldInterface(physics)
		{
			physx::PxSceneDesc sceneDescription(physics->getPhysics()->getTolerancesScale());
			sceneDescription.broadPhaseType = physx::PxBroadPhaseType::eMBP;
			sceneDescription.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
			if (sceneDescription.cpuDispatcher == nullptr)
			{
				sceneDescription.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
				if (sceneDescription.cpuDispatcher == nullptr)
				{
					assert(!"Impossible to create cpu dispatcher");
					return;
				}
			}
			if (sceneDescription.filterShader == nullptr)
			{
				sceneDescription.filterShader = physx::PxDefaultSimulationFilterShader;
			}
			scene = physics->getPhysics()->createScene(sceneDescription);
			assert(scene != nullptr && "Impossible to create scene");
		}

		// Methods
		physx::PxScene *PhysXWorld::getScene(void)
		{
			return scene;
		}

		const physx::PxScene *PhysXWorld::getScene(void) const
		{
			return scene;
		}

		// Inherited Methods
		void PhysXWorld::setGravity(const glm::vec3 &gravity)
		{
			assert(scene != nullptr && "Invalid scene");
			scene->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
		}

		glm::vec3 PhysXWorld::getGravity(void) const
		{
			assert(scene != nullptr && "Invalid scene");
			physx::PxVec3 gravity = scene->getGravity();
			return glm::vec3(gravity.x, gravity.y, gravity.z);
		}

		void PhysXWorld::simulate(float stepSize)
		{
			assert(scene != nullptr && "Invalid scene");
			scene->simulate(stepSize);
			scene->fetchResults(true);
		}

		RigidBodyInterface *PhysXWorld::createRigidBody(const glm::vec3 &position)
		{
			return static_cast<RigidBodyInterface *>(new PhysXRigidBody(this, position));
		}

		MaterialInterface *PhysXWorld::createMaterial(void)
		{
			return static_cast<MaterialInterface *>(new PhysXMaterial(this));
		}
	}
}