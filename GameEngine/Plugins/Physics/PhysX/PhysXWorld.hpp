#pragma once

#include "WorldInterface.hpp"
#include "PhysXPhysics.hpp"
#include "PhysXRigidBody.hpp"
#include "PhysXMaterial.hpp"
#include "PhysXBoxCollider.hpp"
#include "PhysXCapsuleCollider.hpp"
#include "PhysXMeshCollider.hpp"
#include "PhysXSphereCollider.hpp"
#include "MemoryPoolHelper.hpp"
#include "CollisionListener.hpp"
#include "TriggerListener.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXWorld final : public WorldInterface, public physx::PxSimulationEventCallback, public physx::PxBroadPhaseCallback, public MemoryPoolHelper<PhysXRigidBody, PhysXMaterial, PhysXBoxCollider, PhysXCapsuleCollider, PhysXMeshCollider, PhysXSphereCollider>
		{
		public:
			// Constructors
			PhysXWorld(void) = delete;

			PhysXWorld(PhysXPhysics *physics);

			PhysXWorld(const PhysXWorld &) = delete;

			// Assignment Operators
			PhysXWorld &operator=(const PhysXWorld &) = delete;

			// Methods
			physx::PxScene *getScene(void);

			const physx::PxScene *getScene(void) const;

		private:
			// Attributes
			physx::PxScene *scene = nullptr;

			physx::PxU32 groupCollisionFlags[32];

			__declspec(align(16)) std::uint8_t scratchMemoryBlock[65536];

			physx::PxDefaultCpuDispatcher *cpuDispatcher = nullptr;

			physx::PxCudaContextManager *cudaContextManager = nullptr;

			std::unordered_map<Collider *, std::unordered_map<Collider *, std::size_t>> triggers;

			// Static Methods
			static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attributes2,
													 physx::PxFilterData filterData2, physx::PxPairFlags& pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize);

			// Destructor
			~PhysXWorld(void);

			// Methods
			void notifyTriggers(void);

			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(Private::GenericData *data) override final;

			void destroyRigidBody(RigidBodyInterface *rigidBody) override final;

			ColliderInterface *createCollider(ColliderType colliderType, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data) override final;

			void destroyCollider(ColliderInterface *collider) override final;

			MaterialInterface *createMaterial(ColliderInterface *collider) override final;

			void destroyMaterial(MaterialInterface *material) override final;

			void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 numberOfConstraints) override final;

			void onWake(physx::PxActor **actors, physx::PxU32 numberOfActors) override final;

			void onSleep(physx::PxActor **actors, physx::PxU32 numberOfActors) override final;

			void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 numberOfPairs) override final;

			void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 numberOfPairs) override final;

			void onObjectOutOfBounds(physx::PxShape &shape, physx::PxActor &actor) override final;

			void onObjectOutOfBounds(physx::PxAggregate &aggregate) override final;
		};
	}
}
