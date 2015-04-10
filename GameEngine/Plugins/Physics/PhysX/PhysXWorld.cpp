#include <thread>
#include <cassert>

#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		PhysXWorld::PhysXWorld(PhysXPlugin *plugin, const glm::vec3 &gravity)
			: WorldInterface(static_cast<PhysicsInterface *>(plugin))
		{
			physx::PxSceneDesc sceneDescription(plugin->getPhysics()->getTolerancesScale());
			sceneDescription.broadPhaseType = physx::PxBroadPhaseType::eMBP;
			sceneDescription.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
			if (sceneDescription.cpuDispatcher == nullptr)
			{
				sceneDescription.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
				if (sceneDescription.cpuDispatcher == nullptr)
				{
					// Log Error
					return;
				}
			}
			if (sceneDescription.filterShader == nullptr)
			{
				sceneDescription.filterShader = physx::PxDefaultSimulationFilterShader;
			}
			scene = plugin->getPhysics()->createScene(sceneDescription);
			if (scene == nullptr)
			{
				// Log Error
			}
		}

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
	}
}