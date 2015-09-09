#include "PhysXPhysics.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		physx::PxFoundation *PhysXPhysics::foundation = nullptr;
		physx::PxPhysics *PhysXPhysics::physics = nullptr;
		bool PhysXPhysics::extensions = false;
		std::size_t PhysXPhysics::counter = 0;

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
			if (foundation == nullptr)
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
			if (physics == nullptr)
			{
				physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale);
				if (physics == nullptr)
				{
					assert(!"Impossible to create physics");
					return false;
				}
			}
			if (!extensions)
			{
				extensions = PxInitExtensions(*physics);
				if (!extensions)
				{
					assert(!"Impossible to initialize extensions");
					return false;
				}
			}
			++counter;
			return true;
		}

		void PhysXPhysics::finalize(void)
		{
			if (!--counter)
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
		}

		WorldInterface *PhysXPhysics::createWorld(void)
		{
			return new PhysXWorld(this);
		}
	}
}
