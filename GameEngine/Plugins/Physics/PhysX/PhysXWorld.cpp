#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

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
#include "PhysXRaycaster.hpp"

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
			for (auto &shapePair : collisionShapes)
			{
				shapePair.second.second->release();
				//delete[] shapePair.second.first;
			}
			collisionShapes.clear();
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

		physx::PxShape *PhysXWorld::getCollisionShape(const std::string &mesh, bool isConvex)
		{
			const std::string path = getAssetManager()->getAssetsDirectory() + mesh + "_physx" + (isConvex ? "_convex" : "_concave") + ".phage";
			auto found = collisionShapes.find(path);
			if (found != collisionShapes.end())
			{
				physx::PxPhysics *physics = static_cast<PhysXPhysics *>(getPhysics())->getPhysics();
				physx::PxMaterial *material = static_cast<PhysXMaterial *>(getMaterial(GetDefaultMaterialName()))->getMaterial();
				return found->second.second->getConcreteType() == physx::PxConcreteType::eTRIANGLE_MESH ?
					physics->createShape(physx::PxTriangleMeshGeometry(static_cast<physx::PxTriangleMesh *>(found->second.second)), *material, true) :
					physics->createShape(physx::PxConvexMeshGeometry(static_cast<physx::PxConvexMesh *>(found->second.second)), *material, true);
			}
			OldFile filePath(path);
			if (!filePath.exists())
			{
				std::cerr << "Physics file not found." << std::endl;
				return nullptr;
			}
			FILE *stream = nullptr;
			fopen_s(&stream, path.c_str(), "rb");
			if (stream == nullptr)
			{
				std::cerr << "Impossible to open PhysX file." << std::endl;
				return nullptr;
			}
			std::fseek(stream, 0, SEEK_END);
			const std::size_t fileSize = std::ftell(stream);
			if (fileSize == 0U)
			{
				std::cerr << "PhysX file is invalid. File is empty." << std::endl;
				std::fclose(stream);
				return nullptr;
			}
			std::fseek(stream, 0, SEEK_SET);
			void *collectionBuffer = std::malloc(fileSize + PX_SERIAL_FILE_ALIGN);
			void *bytes = reinterpret_cast<void *>((reinterpret_cast<std::size_t>(collectionBuffer) + PX_SERIAL_FILE_ALIGN) & ~(PX_SERIAL_FILE_ALIGN - 1));
			std::fread(bytes, 1, fileSize, stream);
			std::fclose(stream);
			physx::PxSerializationRegistry *registry = physx::PxSerialization::createSerializationRegistry(*static_cast<PhysXPhysics *>(getPhysics())->getPhysics());
			physx::PxCollection *collection = physx::PxSerialization::createCollectionFromBinary(bytes, *registry);
			if (collection->getNbObjects() != 1 || (collection->getObject(0).getConcreteType() != physx::PxConcreteType::eTRIANGLE_MESH &&
													collection->getObject(0).getConcreteType() != physx::PxConcreteType::eCONVEX_MESH))
			{
				std::cerr << "PhysX file is invalid. No collision shape inside." << std::endl;
				return nullptr;
			}
			collisionShapes.emplace(std::make_pair(path, std::make_pair(collectionBuffer, &collection->getObject(0))));
			collection->release();
			registry->release();
			return getCollisionShape(mesh, isConvex);
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

		RaycasterInterface *PhysXWorld::createRaycaster(void)
		{
			return new PhysXRaycaster(this);
		}

		RigidBodyInterface *PhysXWorld::createRigidBody(Private::GenericData *data)
		{
			return create<PhysXRigidBody>(this, data);
		}

		void PhysXWorld::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			destroy(static_cast<PhysXRigidBody *>(rigidBody));
		}

		ColliderInterface *PhysXWorld::createCollider(ColliderType colliderType, const std::string &mesh, Private::GenericData *data)
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

		MaterialInterface *PhysXWorld::createMaterial(const std::string &name)
		{
			MaterialTable::iterator found = materials.find(name);
			if (found != materials.end())
			{
				++found->second.second;
				return found->second.first;
			}
			else
			{
				return materials.insert(std::make_pair(name, std::make_pair(create<PhysXMaterial>(this, name), 1))).first->second.first;
			}
		}

		void PhysXWorld::destroyMaterial(MaterialInterface *material)
		{
			assert(material != nullptr && "Invalid material");
			MaterialTable::iterator found = materials.find(material->getName());
			if (found != materials.end())
			{
				--found->second.second;
				if (found->second.second == 0)
				{
					destroy(static_cast<PhysXMaterial *>(material));
					materials.erase(found);
				}
			}
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