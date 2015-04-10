#include "PhysXPlugin.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		physx::PxFoundation *PhysXPlugin::getFoundation(void)
		{
			return foundation;
		}

		physx::PxCooking *PhysXPlugin::getCooking(void)
		{
			return cooking;
		}

		physx::PxPhysics *PhysXPlugin::getPhysics(void)
		{
			return physics;
		}

		// Inherited Methods
		EngineTypes PhysXPlugin::getPluginType(void) const
		{
			return EngineTypes::PhysX;
		}

		bool PhysXPlugin::initialize(void)
		{
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallback, defaultErrorCallback);
			if (foundation == nullptr)
			{
				// Log Error
				return false;
			}
			cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(toleranceScale));
			if (cooking == nullptr)
			{
				// Log Error
				return false;
			}
			physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale);
			if (physics == nullptr)
			{
				// Log Error
				return false;
			}
			if (!PxInitExtensions(*physics))
			{
				// Log Error
				return false;
			}
			return true;
		}

		void PhysXPlugin::finalize(void)
		{
			if (physics != nullptr)
			{
				physics->release();
				physics = nullptr;
			}
			if (cooking != nullptr)
			{
				cooking->release();
				cooking = nullptr;
			}
			if (foundation != nullptr)
			{
				foundation->release();
				foundation = nullptr;
			}
		}

		WorldInterface *PhysXPlugin::createWorld(const glm::vec3 &gravity, const std::string &worldName)
		{
			return new PhysXWorld(this, gravity, worldName);
		}

		void PhysXPlugin::deleteWorld(WorldInterface *world)
		{
			delete static_cast<PhysXWorld *>(world);
		}
	}
}
