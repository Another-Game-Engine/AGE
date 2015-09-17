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
#include "PxFiltering.h"
#include "characterkinematic/PxCapsuleController.h"

#include "PhysXCharacterController.hpp"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		physx::PxFilterFlags PhysXWorld::FilterShader(physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attributes2,
													  physx::PxFilterData filterData2, physx::PxPairFlags &pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize)
		{
			pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			const bool kinematic0 = physx::PxFilterObjectIsKinematic(attributes1);
			const bool kinematic1 = physx::PxFilterObjectIsKinematic(attributes2);
			if (kinematic0 && kinematic1)
			{
				pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
				return physx::PxFilterFlag::eSUPPRESS;
			}
			else if (physx::PxFilterObjectIsTrigger(attributes1) || physx::PxFilterObjectIsTrigger(attributes2))
			{
				pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;
				return physx::PxFilterFlag::eDEFAULT;
			}
			pairFlags |= physx::PxPairFlag::eRESOLVE_CONTACTS | physx::PxPairFlag::eCCD_LINEAR | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
			const PhysXWorld *world = reinterpret_cast<const PhysXWorld *>(static_cast<const physx::PxU64 *>(constantBlock)[0]);
			return world->groupCollisionFlags[PhysXWorld::GetIndexForFilterGroup(static_cast<FilterGroup>(filterData1.word0))] & filterData2.word0 ? physx::PxFilterFlag::eDEFAULT : physx::PxFilterFlag::eSUPPRESS;
		}

		physx::PxU32 PhysXWorld::GetIndexForFilterGroup(FilterGroup group)
		{
			physx::PxU32 groupValue = static_cast<physx::PxU32>(group);
			physx::PxU32 index = physx::PxU32(0);
			while (groupValue >>= 1)
			{
				++index;
			}
			return index;
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
			worldAddress[0] = reinterpret_cast<physx::PxU64>(this);
			sceneDescription.simulationEventCallback = this;
			sceneDescription.broadPhaseCallback = this;
			sceneDescription.filterShader = &PhysXWorld::FilterShader;
			sceneDescription.filterShaderData = static_cast<const void *>(worldAddress);
			sceneDescription.filterShaderDataSize = sizeof(worldAddress);
			scene = physics->getPhysics()->createScene(sceneDescription);
			assert(scene != nullptr && "Impossible to create scene");
		}

		// Destructor
		PhysXWorld::~PhysXWorld(void)
		{
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

		std::vector<physx::PxShape *> PhysXWorld::getCollisionShapes(const std::string &mesh, bool isConvex)
		{
			std::vector<physx::PxShape *> shapes;
			const std::string path = getAssetManager()->getAssetsDirectory() + mesh + "_physx" + (isConvex ? "_convex" : "_concave") + ".phage";
			auto found = collisionShapes.find(path);
			if (found != collisionShapes.end())
			{
				physx::PxPhysics *physics = static_cast<PhysXPhysics *>(getPhysics())->getPhysics();
				physx::PxMaterial *material = static_cast<PhysXMaterial *>(getMaterial(GetDefaultMaterialName()))->getMaterial();
				for (physx::PxBase *base : found->second.second)
				{
					shapes.push_back(base->getConcreteType() == physx::PxConcreteType::eTRIANGLE_MESH ?
									 physics->createShape(physx::PxTriangleMeshGeometry(static_cast<physx::PxTriangleMesh *>(base)), *material, true) :
									 physics->createShape(physx::PxConvexMeshGeometry(static_cast<physx::PxConvexMesh *>(base)), *material, true));
				}
				return std::move(shapes);
			}
			OldFile filePath(path);
			if (!filePath.exists())
			{
				std::cerr << "Physics file not found." << std::endl;
				return std::move(shapes);
			}
			FILE *stream = nullptr;
			fopen_s(&stream, path.c_str(), "rb");
			if (stream == nullptr)
			{
				std::cerr << "Impossible to open PhysX file." << std::endl;
				return std::move(shapes);
			}
			std::fseek(stream, 0, SEEK_END);
			const std::size_t fileSize = std::ftell(stream);
			if (fileSize == 0U)
			{
				std::cerr << "PhysX file is invalid. File is empty." << std::endl;
				std::fclose(stream);
				return std::move(shapes);
			}
			std::fseek(stream, 0, SEEK_SET);
			void *collectionBuffer = std::malloc(fileSize + PX_SERIAL_FILE_ALIGN);
			void *bytes = reinterpret_cast<void *>((reinterpret_cast<std::size_t>(collectionBuffer) + PX_SERIAL_FILE_ALIGN) & ~(PX_SERIAL_FILE_ALIGN - 1));
			std::fread(bytes, 1, fileSize, stream);
			std::fclose(stream);
			physx::PxSerializationRegistry *registry = physx::PxSerialization::createSerializationRegistry(*static_cast<PhysXPhysics *>(getPhysics())->getPhysics());
			physx::PxCollection *collection = physx::PxSerialization::createCollectionFromBinary(bytes, *registry);
			std::vector<physx::PxBase *> baseShapes;
			for (physx::PxU32 objectIndex = 0; objectIndex < collection->getNbObjects(); ++objectIndex)
			{
				if (collection->getObject(objectIndex).getConcreteType() != physx::PxConcreteType::eTRIANGLE_MESH && collection->getObject(objectIndex).getConcreteType() != physx::PxConcreteType::eCONVEX_MESH)
				{
					std::cerr << "PhysX file is invalid. No collision shape inside." << std::endl;
					return std::move(shapes);
				}
				baseShapes.push_back(&collection->getObject(objectIndex));
			}
			collisionShapes.emplace(std::make_pair(path, std::make_pair(collectionBuffer, std::move(baseShapes))));
			collection->release();
			registry->release();
			return getCollisionShapes(mesh, isConvex);
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
			groupCollisionFlags[PhysXWorld::GetIndexForFilterGroup(group1)] |= static_cast<physx::PxU32>(group2);
			groupCollisionFlags[PhysXWorld::GetIndexForFilterGroup(group2)] |= static_cast<physx::PxU32>(group1);
		}

		void PhysXWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[PhysXWorld::GetIndexForFilterGroup(group1)] &= static_cast<physx::PxU32>(~group2);
			groupCollisionFlags[PhysXWorld::GetIndexForFilterGroup(group2)] &= static_cast<physx::PxU32>(~group1);
		}

		void PhysXWorld::simulate(float stepSize)
		{
			assert(scene != nullptr && "Invalid scene");
			if (stepSize > std::numeric_limits<float>::epsilon())
			{
				scene->simulate(stepSize, nullptr, scratchMemoryBlock, sizeof(scratchMemoryBlock));
				scene->fetchResults(true);
				notifyTriggers();
				fillDebugInformation();
			}
		}

		void PhysXWorld::fillDebugInformation(void)
		{
			if (isDebugEnabled())
			{
				const physx::PxRenderBuffer &renderBuffer = scene->getRenderBuffer();
				for (physx::PxU32 index = 0; index < renderBuffer.getNbPoints(); ++index)
				{
					const physx::PxDebugPoint &point = renderBuffer.getPoints()[index];
					DebugInformation::Point debugPoint;
					debugPoint.color[0].r = static_cast<float>((point.color & 0xFF0000) >> 16) / 255.0f;
					debugPoint.color[0].g = static_cast<float>((point.color & 0xFF00) >> 8) / 255.0f;
					debugPoint.color[0].b = static_cast<float>(point.color & 0xFF) / 255.0f;
					debugPoint.position[0] = glm::vec3(static_cast<float>(point.pos.x), static_cast<float>(point.pos.y), static_cast<float>(point.pos.z));
					debugInformation.points.push_back(std::move(debugPoint));
				}
				for (physx::PxU32 index = 0; index < renderBuffer.getNbLines(); ++index)
				{
					const physx::PxDebugLine &line = renderBuffer.getLines()[index];
					DebugInformation::Line debugLine;
					debugLine.color[0].r = static_cast<float>((line.color0 & 0xFF0000) >> 16) / 255.0f;
					debugLine.color[0].g = static_cast<float>((line.color0 & 0xFF00) >> 8) / 255.0f;
					debugLine.color[0].b = static_cast<float>(line.color0 & 0xFF) / 255.0f;
					debugLine.position[0] = glm::vec3(static_cast<float>(line.pos0.x), static_cast<float>(line.pos0.y), static_cast<float>(line.pos0.z));
					debugLine.color[1].r = static_cast<float>((line.color1 & 0xFF0000) >> 16) / 255.0f;
					debugLine.color[1].g = static_cast<float>((line.color1 & 0xFF00) >> 8) / 255.0f;
					debugLine.color[1].b = static_cast<float>(line.color1 & 0xFF) / 255.0f;
					debugLine.position[1] = glm::vec3(static_cast<float>(line.pos1.x), static_cast<float>(line.pos1.y), static_cast<float>(line.pos1.z));
					debugInformation.lines.push_back(std::move(debugLine));
				}
				for (physx::PxU32 index = 0; index < renderBuffer.getNbTriangles(); ++index)
				{
					const physx::PxDebugTriangle &triangle = renderBuffer.getTriangles()[index];
					DebugInformation::Triangle debugTriangle;
					debugTriangle.color[0].r = static_cast<float>((triangle.color0 & 0xFF0000) >> 16) / 255.0f;
					debugTriangle.color[0].g = static_cast<float>((triangle.color0 & 0xFF00) >> 8) / 255.0f;
					debugTriangle.color[0].b = static_cast<float>(triangle.color0 & 0xFF) / 255.0f;
					debugTriangle.position[0] = glm::vec3(static_cast<float>(triangle.pos0.x), static_cast<float>(triangle.pos0.y), static_cast<float>(triangle.pos0.z));
					debugTriangle.color[1].r = static_cast<float>((triangle.color1 & 0xFF0000) >> 16) / 255.0f;
					debugTriangle.color[1].g = static_cast<float>((triangle.color1 & 0xFF00) >> 8) / 255.0f;
					debugTriangle.color[1].b = static_cast<float>(triangle.color1 & 0xFF) / 255.0f;
					debugTriangle.position[1] = glm::vec3(static_cast<float>(triangle.pos1.x), static_cast<float>(triangle.pos1.y), static_cast<float>(triangle.pos1.z));
					debugTriangle.color[2].r = static_cast<float>((triangle.color2 & 0xFF0000) >> 16) / 255.0f;
					debugTriangle.color[2].g = static_cast<float>((triangle.color2 & 0xFF00) >> 8) / 255.0f;
					debugTriangle.color[2].b = static_cast<float>(triangle.color2 & 0xFF) / 255.0f;
					debugTriangle.position[2] = glm::vec3(static_cast<float>(triangle.pos2.x), static_cast<float>(triangle.pos2.y), static_cast<float>(triangle.pos2.z));
					debugInformation.triangles.push_back(std::move(debugTriangle));
				}
			}
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

		CharacterControllerInterface *PhysXWorld::createCharacterController()
		{
			return (new PhysXCharacterController(this));
		}

		void PhysXWorld::destroyCharacterController(CharacterControllerInterface *cc)
		{
			delete cc;
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

		void PhysXWorld::enableDebug(void)
		{
			WorldInterface::enableDebug();
			assert(scene != nullptr && "Invalid scene");
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eWORLD_AXES, 15.0f);
			/*scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_AXES, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_MASS_AXES, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_ANG_VELOCITY, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_POINT, 1.0f);*/
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_NORMAL, 5.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_FORCE, 5.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 2.5f);
			//scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_AABBS, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			//scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_AXES, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_EDGES, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_STATIC, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_DYNAMIC, 1.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_PAIRS, 1.0f);

		}

		void PhysXWorld::disableDebug(void)
		{
			WorldInterface::disableDebug();
			assert(scene != nullptr && "Invalid scene");
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eWORLD_AXES, 0.0f);
			/*scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_AXES, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_MASS_AXES, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_LIN_VELOCITY, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_ANG_VELOCITY, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_POINT, 0.0f);*/
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_NORMAL, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCONTACT_FORCE, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 0.0f);
			//scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_AABBS, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 0.0f);
			//scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_AXES, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_EDGES, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_STATIC, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_DYNAMIC, 0.0f);
			scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_PAIRS, 0.0f);
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
			if (pairHeader.actors[0]->userData == nullptr ||
				pairHeader.actors[1]->userData == nullptr)
				return;
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