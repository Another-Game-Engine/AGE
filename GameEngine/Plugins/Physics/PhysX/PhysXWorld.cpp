#include <thread>
#include <cassert>

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
		// Static Variables
		static physx::PxU32 GroupCollisionFlags[32];

		// Static Methods
		physx::PxFilterFlags PhysXWorld::FilterShader(physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attributes2,
													  physx::PxFilterData filterData2, physx::PxPairFlags &pairFlags, const void *constantBlock, physx::PxU32 constantBlockSize)
		{
			if (physx::PxFilterObjectIsTrigger(attributes1) || physx::PxFilterObjectIsTrigger(attributes2))
			{
				pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
				return physx::PxFilterFlag::eDEFAULT;
			}
			pairFlags = physx::PxPairFlag::eRESOLVE_CONTACTS | physx::PxPairFlag::eCCD_LINEAR;
			const physx::PxU32 shapeGroup1 = filterData1.word0 & 31;
			const physx::PxU32 shapeGroup2 = filterData2.word0 & 31;
			const physx::PxU32 *groupCollisionFlags = static_cast<const physx::PxU32 *>(constantBlock);
			return (groupCollisionFlags[shapeGroup1] & (1 << shapeGroup2)) == 0 ? physx::PxFilterFlag::eSUPPRESS : physx::PxFilterFlag::eDEFAULT;
		}

		// Constructors
		PhysXWorld::PhysXWorld(PhysXPhysics *physics, const glm::vec3 &gravity)
			: WorldInterface(physics)
		{
			physx::PxSceneDesc sceneDescription(physics->getPhysics()->getTolerancesScale());
			sceneDescription.flags |= physx::PxSceneFlag::eENABLE_CCD;
			sceneDescription.broadPhaseType = physx::PxBroadPhaseType::eMBP;
			sceneDescription.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
			sceneDescription.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
			if (sceneDescription.cpuDispatcher == nullptr)
			{
				assert(!"Impossible to create cpu dispatcher");
				return;
			}
			for (std::size_t index = 0; index < sizeof(GroupCollisionFlags) / sizeof(*GroupCollisionFlags); ++index)
			{
				GroupCollisionFlags[index] = 0xFFFFFFFF;
			}
			sceneDescription.filterShader = &PhysXWorld::FilterShader;
			sceneDescription.filterShaderData = static_cast<const void *>(GroupCollisionFlags);
			sceneDescription.filterShaderDataSize = sizeof(GroupCollisionFlags);
			scene = physics->getPhysics()->createScene(sceneDescription);
			assert(scene != nullptr && "Impossible to create scene");
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
			GroupCollisionFlags[static_cast<std::uint8_t>(group1)] |= (1 << static_cast<std::uint8_t>(group2));
			GroupCollisionFlags[static_cast<std::uint8_t>(group2)] |= (1 << static_cast<std::uint8_t>(group1));
		}

		void PhysXWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			GroupCollisionFlags[static_cast<std::uint8_t>(group1)] &= ~(1 << static_cast<std::uint8_t>(group2));
			GroupCollisionFlags[static_cast<std::uint8_t>(group2)] &= ~(1 << static_cast<std::uint8_t>(group1));
		}

		void PhysXWorld::simulate(float stepSize)
		{
			assert(scene != nullptr && "Invalid scene");
			scene->simulate(stepSize);
			scene->fetchResults(true);
		}

		RigidBodyInterface *PhysXWorld::createRigidBody(Private::GenericData *data)
		{
			return create<PhysXRigidBody>(this, data);
		}

		void PhysXWorld::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			destroy(static_cast<PhysXRigidBody *>(rigidBody));
		}

		ColliderInterface *PhysXWorld::createCollider(ColliderType colliderType, Private::GenericData *data)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return create<PhysXBoxCollider>(this, data);
				case ColliderType::Capsule:
					return create<PhysXCapsuleCollider>(this, data);
				case ColliderType::Mesh:
					return create<PhysXMeshCollider>(this, data);
				case ColliderType::Sphere:
					return create<PhysXSphereCollider>(this, data);
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		void PhysXWorld::destroyCollider(ColliderInterface *collider)
		{
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
	}
}