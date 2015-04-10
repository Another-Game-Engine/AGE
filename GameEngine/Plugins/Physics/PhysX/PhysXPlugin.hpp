#pragma once

#include <PxPhysicsAPI.h>

#include "PluginInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXPlugin final : public PluginInterface
		{
		public:
			// Constructors
			PhysXPlugin(void) = default;

			PhysXPlugin(const PhysXPlugin &) = delete;

			// Assignment Operators
			PhysXPlugin &operator=(const PhysXPlugin &) = delete;

			// Destructor
			virtual ~PhysXPlugin(void) = default;

			// Methods
			physx::PxFoundation *getFoundation(void);

			physx::PxCooking *getCooking(void);

			physx::PxPhysics *getPhysics(void);

		private:
			// Attributes
			physx::PxDefaultErrorCallback defaultErrorCallback;
			
			physx::PxDefaultAllocator defaultAllocatorCallback;

			physx::PxTolerancesScale toleranceScale;
			
			physx::PxFoundation *foundation = nullptr;
			
			physx::PxCooking *cooking = nullptr;
			
			physx::PxPhysics *physics = nullptr;

			// Inherited Methods
			EngineTypes getPluginType(void) const override final;

			bool initialize(void) override final;

			void finalize(void) override final;

			WorldInterface *createWorld(const glm::vec3 &gravity, const std::string &worldName) override final;

			void deleteWorld(WorldInterface *world) override final;
		};
	}
}
