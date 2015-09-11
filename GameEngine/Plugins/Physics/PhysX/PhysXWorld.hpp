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

			std::vector<physx::PxShape *> getCollisionShapes(const std::string &mesh, bool isConvex);

			CharacterControllerInterface *createCharacterController() override final;

			void destroyCharacterController(CharacterControllerInterface *cc) override final;

		private:
			// Attributes
			physx::PxScene *scene = nullptr;

			physx::PxU64 worldAddress[1];

			physx::PxU32 groupCollisionFlags[32];

			__declspec(align(16)) std::uint8_t scratchMemoryBlock[65536];

			physx::PxDefaultCpuDispatcher *cpuDispatcher = nullptr;

			physx::PxCudaContextManager *cudaContextManager = nullptr;

			std::unordered_map<Collider *, std::unordered_map<Collider *, std::size_t>> triggers;

			std::unordered_map<std::string, std::pair<void *, std::vector<physx::PxBase *>>> collisionShapes;

			// Static Methods
			static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attributes2,
													 physx::PxFilterData filterData2, physx::PxPairFlags& pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize);

			static physx::PxU32 GetIndexForFilterGroup(FilterGroup group);

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

			RaycasterInterface *createRaycaster(void) override final;

			RigidBodyInterface *createRigidBody(Private::GenericData *data) override final;

			void destroyRigidBody(RigidBodyInterface *rigidBody) override final;

			ColliderInterface *createCollider(ColliderType colliderType, const std::string &mesh, Private::GenericData *data) override final;

			void destroyCollider(ColliderInterface *collider) override final;

			MaterialInterface *createMaterial(const std::string &name) override final;

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
