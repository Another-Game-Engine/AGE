#include <cassert>
#include <vector>
#include <thread>

#include "PhysXWorld.hpp"
#include "PhysXRigidBody.hpp"
#include "PhysXMaterial.hpp"
#include "PhysXBoxCollider.hpp"
#include "PhysXCapsuleCollider.hpp"
#include "PhysXMeshCollider.hpp"
#include "PhysXSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		physx::PxFilterFlags PhysXWorld::FilterShader(physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attributes2,
													  physx::PxFilterData filterData2, physx::PxPairFlags &pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize)
		{
			pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			if (physx::PxFilterObjectIsTrigger(attributes1) || physx::PxFilterObjectIsTrigger(attributes2))
			{
				pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;
				return physx::PxFilterFlag::eDEFAULT;
			}
			pairFlags |= physx::PxPairFlag::eRESOLVE_CONTACTS | physx::PxPairFlag::eCCD_LINEAR | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
			const physx::PxU32 shapeGroup1 = filterData1.word0 & 31;
			const physx::PxU32 shapeGroup2 = filterData2.word0 & 31;
			const physx::PxU32 *groupCollisionFlags = static_cast<const physx::PxU32 *>(constantBlock);
			return (groupCollisionFlags[shapeGroup1] & (1 << shapeGroup2)) == 0 ? physx::PxFilterFlag::eSUPPRESS : physx::PxFilterFlag::eDEFAULT;
		}

		// Constructors
		PhysXWorld::PhysXWorld(PhysXPhysics *physics)
			: WorldInterface(physics)
		{
			physx::PxSceneDesc sceneDescription(physics->getPhysics()->getTolerancesScale());
			sceneDescription.flags |= physx::PxSceneFlag::eENABLE_CCD | physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS | physx::PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;
			sceneDescription.broadPhaseType = physx::PxBroadPhaseType::eSAP;
			sceneDescription.frictionType = physx::PxFrictionType::eTWO_DIRECTIONAL;
			sceneDescription.gravity = physx::PxVec3(GetDefaultGravity().x, GetDefaultGravity().y, GetDefaultGravity().z);
			cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
			sceneDescription.cpuDispatcher = cpuDispatcher;
			cudaContextManager = physx::PxCreateCudaContextManager(*physics->getFoundation(), physx::PxCudaContextManagerDesc(), nullptr);
			sceneDescription.gpuDispatcher = cudaContextManager->getGpuDispatcher();
			if (sceneDescription.cpuDispatcher == nullptr)
			{
				assert(!"Impossible to create cpu dispatcher");
				return;
			}
			for (std::size_t index = 0; index < sizeof(groupCollisionFlags) / sizeof(*groupCollisionFlags); ++index)
			{
				groupCollisionFlags[index] = 0xFFFFFFFF;
			}
			sceneDescription.simulationEventCallback = this;
			sceneDescription.broadPhaseCallback = this;
			sceneDescription.filterShader = &PhysXWorld::FilterShader;
			sceneDescription.filterShaderData = static_cast<const void *>(groupCollisionFlags);
			sceneDescription.filterShaderDataSize = sizeof(groupCollisionFlags);
			scene = physics->getPhysics()->createScene(sceneDescription);
			assert(scene != nullptr && "Impossible to create scene");
		}

		// Destructor
		PhysXWorld::~PhysXWorld(void)
		{
			if (scene != nullptr)
			{
				scene->setBroadPhaseCallback(nullptr);
				scene->setSimulationEventCallback(nullptr);
				scene->release();
				scene = nullptr;
			}
			if (cudaContextManager != nullptr)
			{
				cudaContextManager->release();
				cudaContextManager = nullptr;
			}
			if (cpuDispatcher != nullptr)
			{
				cpuDispatcher->release();
				cpuDispatcher = nullptr;
			}
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

		void PhysXWorld::notifyTriggers(void)
		{
			TriggerListener *listener = getTriggerListener();
			for (std::pair<Collider * const, std::unordered_map<Collider *, std::size_t>> &triggerPairs : triggers)
			{
				for (std::pair<Collider * const, std::size_t> &pair : triggerPairs.second)
				{
					if (pair.second == 0)
					{
						listener->onTrigger(triggerPairs.first, pair.first, TriggerType::Persistent);
					}
					else
					{
						pair.second = 0;
					}
				}
			}
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

		void PhysXWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[static_cast<std::uint8_t>(group1)] |= (1 << static_cast<std::uint8_t>(group2));
			groupCollisionFlags[static_cast<std::uint8_t>(group2)] |= (1 << static_cast<std::uint8_t>(group1));
		}

		void PhysXWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[static_cast<std::uint8_t>(group1)] &= ~(1 << static_cast<std::uint8_t>(group2));
			groupCollisionFlags[static_cast<std::uint8_t>(group2)] &= ~(1 << static_cast<std::uint8_t>(group1));
		}

		void PhysXWorld::simulate(float stepSize)
		{
			assert(scene != nullptr && "Invalid scene");
			scene->simulate(stepSize, nullptr, scratchMemoryBlock, sizeof(scratchMemoryBlock));
			scene->fetchResults(true);
			notifyTriggers();
		}

		RigidBodyInterface *PhysXWorld::createRigidBody(Private::GenericData *data)
		{
			return create<PhysXRigidBody>(this, data);
		}

		void PhysXWorld::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			destroy(static_cast<PhysXRigidBody *>(rigidBody));
		}

		ColliderInterface *PhysXWorld::createCollider(ColliderType colliderType, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return create<PhysXBoxCollider>(this, data);
				case ColliderType::Capsule:
					return create<PhysXCapsuleCollider>(this, data);
				case ColliderType::Mesh:
					return create<PhysXMeshCollider>(this, mesh, data);
				case ColliderType::Sphere:
					return create<PhysXSphereCollider>(this, data);
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		void PhysXWorld::destroyCollider(ColliderInterface *collider)
		{
			Collider *colliderComponent = collider->getCollider();
			for (std::pair<Collider * const, std::unordered_map<Collider *, std::size_t>> &triggerPairs : triggers)
			{
				triggerPairs.second.erase(colliderComponent);
			}
			triggers.erase(colliderComponent);
			switch (collider->getColliderType())
			{
				case ColliderType::Box:
					destroy(dynamic_cast<PhysXBoxCollider *>(collider));
					break;
				case ColliderType::Capsule:
					destroy(dynamic_cast<PhysXCapsuleCollider *>(collider));
					break;
				case ColliderType::Mesh:
					destroy(dynamic_cast<PhysXMeshCollider *>(collider));
					break;
				case ColliderType::Sphere:
					destroy(dynamic_cast<PhysXSphereCollider *>(collider));
					break;
				default:
					assert(!"Never reached");
					break;
			}
		}

		MaterialInterface *PhysXWorld::createMaterial(ColliderInterface *collider)
		{
			return create<PhysXMaterial>(this, collider);
		}

		void PhysXWorld::destroyMaterial(MaterialInterface *material)
		{
			destroy(static_cast<PhysXMaterial *>(material));
		}

		void PhysXWorld::onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 numberOfConstraints)
		{
			return;
		}

		void PhysXWorld::onWake(physx::PxActor **actors, physx::PxU32 numberOfActors)
		{
			return;
		}

		void PhysXWorld::onSleep(physx::PxActor **actors, physx::PxU32 numberOfActors)
		{
			return;
		}

		void PhysXWorld::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 numberOfPairs)
		{
			if (pairHeader.flags.isSet(physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0) || pairHeader.flags.isSet(physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
			{
				return;
			}
			assert(pairHeader.actors[0]->userData != nullptr && "Invalid actor");
			assert(pairHeader.actors[1]->userData != nullptr && "Invalid actor");
			CollisionListener *collisionListener = getCollisionListener();
			std::vector<Contact> firstColliderContacts;
			std::vector<Contact> secondColliderContacts;
			CollisionType collisionType = CollisionType::Persistent;
			for (physx::PxU32 index = 0; index < numberOfPairs; ++index)
			{
				const physx::PxContactPair &contactPair = pairs[index];
				if (contactPair.flags.isSet(physx::PxContactPairFlag::eREMOVED_SHAPE_0) || contactPair.flags.isSet(physx::PxContactPairFlag::eREMOVED_SHAPE_1))
				{
					continue;
				}
				if (contactPair.events.isSet(physx::PxPairFlag::eNOTIFY_TOUCH_FOUND))
				{
					collisionType = CollisionType::New;
				}
				else if (contactPair.events.isSet(physx::PxPairFlag::eNOTIFY_TOUCH_LOST))
				{
					collisionType = CollisionType::Lost;
				}
				physx::PxContactStreamIterator iterator(const_cast<physx::PxU8 *>(contactPair.contactStream), contactPair.contactStreamSize);
				while (iterator.hasNextPatch())
				{
					iterator.nextPatch();
					while (iterator.hasNextContact())
					{
						iterator.nextContact();
						const physx::PxVec3 position = iterator.getContactPoint();
						const physx::PxVec3 normal = iterator.getContactNormal();
						firstColliderContacts.push_back({ glm::vec3(position.x, position.y, position.z), glm::vec3(normal.x, normal.y, normal.z) });
						secondColliderContacts.push_back({ glm::vec3(position.x, position.y, position.z), glm::vec3(-normal.x, -normal.y, -normal.z) });
					}
				}
			}
			Collider *firstCollider = static_cast<ColliderInterface *>(static_cast<PhysXCollider *>(pairHeader.actors[0]->userData))->getCollider();
			Collider *secondCollider = static_cast<ColliderInterface *>(static_cast<PhysXCollider *>(pairHeader.actors[1]->userData))->getCollider();
			collisionListener->onCollision(firstCollider, secondCollider, firstColliderContacts, collisionType);
			collisionListener->onCollision(secondCollider, firstCollider, secondColliderContacts, collisionType);
		}

		void PhysXWorld::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 numberOfPairs)
		{
			TriggerListener *triggerListener = getTriggerListener();
			for (physx::PxU32 index = 0; index < numberOfPairs; ++index)
			{
				const physx::PxTriggerPair &triggerPair = pairs[index];
				if (triggerPair.flags.isSet(physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER) || triggerPair.flags.isSet(physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				{
					continue;
				}
				Collider *triggerCollider = static_cast<ColliderInterface *>(static_cast<PhysXCollider *>(triggerPair.triggerActor->userData))->getCollider();
				Collider *otherCollider = static_cast<ColliderInterface *>(static_cast<PhysXCollider *>(triggerPair.otherActor->userData))->getCollider();
				TriggerType triggerType = TriggerType::Persistent;
				if (triggerPair.status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					triggerType = TriggerType::New;
					triggers[triggerCollider][otherCollider] = 1;
				}
				else if (triggerPair.status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					triggerType = TriggerType::Lost;
					std::unordered_map<Collider *, std::size_t> &currentTriggers = triggers[triggerCollider];
					currentTriggers.erase(otherCollider);
					if (currentTriggers.empty())
					{
						triggers.erase(triggerCollider);
					}
				}
				triggerListener->onTrigger(triggerCollider, otherCollider, triggerType);
			}
		}

		void PhysXWorld::onObjectOutOfBounds(physx::PxShape &shape, physx::PxActor &actor)
		{
			return;
		}

		void PhysXWorld::onObjectOutOfBounds(physx::PxAggregate &aggregate)
		{
			return;
		}
	}
}