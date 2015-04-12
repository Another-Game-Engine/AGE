#pragma once

#include "WorldInterface.hpp"
#include "PhysXPhysics.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXWorld final : public WorldInterface
		{
		public:
			// Constructors
			PhysXWorld(void) = delete;

			PhysXWorld(PhysXPhysics *physics, const glm::vec3 &gravity);

			PhysXWorld(const PhysXWorld &) = delete;

			// Assignment Operators
			PhysXWorld &operator=(const PhysXWorld &) = delete;

			// Destructor
			virtual ~PhysXWorld(void) = default;

			// Methods
			physx::PxScene *getScene(void);

			const physx::PxScene *getScene(void) const;

		private:
			// Attributes
			physx::PxScene *scene = nullptr;

			// Static Methods
			static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attributes2,
													 physx::PxFilterData filterData2, physx::PxPairFlags& pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize);

			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(const glm::vec3 &position) override final;

			ColliderInterface *createCollider(ColliderType colliderType) override final;

			MaterialInterface *createMaterial(ColliderInterface *collider) override final;
		};
	}
}
