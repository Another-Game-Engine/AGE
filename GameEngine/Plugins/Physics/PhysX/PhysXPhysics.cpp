#include "PhysXPhysics.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Methods
		physx::PxFoundation *PhysXPhysics::getFoundation(void)
		{
			return foundation;
		}

		physx::PxCooking *PhysXPhysics::getCooking(void)
		{
			return cooking;
		}

		physx::PxPhysics *PhysXPhysics::getPhysics(void)
		{
			return physics;
		}

		// Inherited Methods
		EngineType PhysXPhysics::getPluginType(void) const
		{
			return EngineType::PhysX;
		}

		bool PhysXPhysics::initialize(void)
		{
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallback, defaultErrorCallback);
			if (foundation == nullptr)
			{
				foundation = &PxGetFoundation();
				if (foundation == nullptr)
				{
					assert(!"Impossible to create foundation");
					return false;
				}
			}
			cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(toleranceScale));
			if (cooking == nullptr)
			{
				assert(!"Impossible to create cooking");
				return false;
			}
			physx::PxCookingParams cookingParameters = cooking->getParams();
			cookingParameters.meshPreprocessParams.set(physx::PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES);
			cookingParameters.meshPreprocessParams.set(physx::PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES);
			cooking->setParams(cookingParameters);
			physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale);
			if (physics == nullptr)
			{
				assert(!"Impossible to create physics");
				return false;
			}
			extensions = PxInitExtensions(*physics);
			if (!extensions)
			{
				assert(!"Impossible to initialize extensions");
				return false;
			}
			return true;
		}

		void PhysXPhysics::finalize(void)
		{
			if (extensions)
			{
				PxCloseExtensions();
				extensions = false;
			}
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

		WorldInterface *PhysXPhysics::createWorld(void)
		{
			return new PhysXWorld(this);
		}
	}
}
