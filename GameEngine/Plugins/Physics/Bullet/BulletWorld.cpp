#include <cassert>

#include "BulletWorld.hpp"
#include "BulletRigidBody.hpp"
#include "BulletMaterial.hpp"
#include "BulletBoxCollider.hpp"
#include "BulletCapsuleCollider.hpp"
#include "BulletMeshCollider.hpp"
#include "BulletSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletWorld::BulletWorld(BulletPhysics *physics, const glm::vec3 &gravity)
			: WorldInterface(physics)
		{
			world.setGravity(btVector3(0.0f, -9.81f, 0.0f));
			world.getPairCache()->setOverlapFilterCallback(this);
			world.getDispatchInfo().m_useContinuous = true;
		}

		// Destructor
		BulletWorld::~BulletWorld(void)
		{
			world.getPairCache()->setOverlapFilterCallback(nullptr);
		}

		// Inherited Methods
		void BulletWorld::setGravity(const glm::vec3 &gravity)
		{
			world.setGravity(btVector3(gravity.x, gravity.y, gravity.z));
		}

		glm::vec3 BulletWorld::getGravity(void) const
		{
			const btVector3 gravity = world.getGravity();
			return glm::vec3(gravity.x(), gravity.y(), gravity.z());
		}

		void BulletWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[static_cast<std::uint8_t>(group1)] |= (1 << static_cast<std::uint8_t>(group2));
			groupCollisionFlags[static_cast<std::uint8_t>(group2)] |= (1 << static_cast<std::uint8_t>(group1));
		}

		void BulletWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			groupCollisionFlags[static_cast<std::uint8_t>(group1)] &= ~(1 << static_cast<std::uint8_t>(group2));
			groupCollisionFlags[static_cast<std::uint8_t>(group2)] &= ~(1 << static_cast<std::uint8_t>(group1));
		}

		void BulletWorld::simulate(float stepSize)
		{
			world.stepSimulation(stepSize);
		}

		RigidBodyInterface *BulletWorld::createRigidBody(Private::GenericData *data)
		{
			return create<BulletRigidBody>(this, data);
		}

		void BulletWorld::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			destroy(static_cast<BulletRigidBody *>(rigidBody));
		}

		ColliderInterface *BulletWorld::createCollider(ColliderType colliderType, Private::GenericData *data)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return create<BulletBoxCollider>(this, data);
				case ColliderType::Capsule:
					return create<BulletCapsuleCollider>(this, data);
				case ColliderType::Mesh:
					return create<BulletMeshCollider>(this, data);
				case ColliderType::Sphere:
					return create<BulletSphereCollider>(this, data);
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		void BulletWorld::destroyCollider(ColliderInterface *collider)
		{
			switch (collider->getColliderType())
			{
				case ColliderType::Box:
					destroy(dynamic_cast<BulletBoxCollider *>(collider));
					break;
				case ColliderType::Capsule:
					destroy(dynamic_cast<BulletCapsuleCollider *>(collider));
					break;
				case ColliderType::Mesh:
					destroy(dynamic_cast<BulletMeshCollider *>(collider));
					break;
				case ColliderType::Sphere:
					destroy(dynamic_cast<BulletSphereCollider *>(collider));
					break;
				default:
					assert(!"Never reached");
					break;
			}
		}

		MaterialInterface *BulletWorld::createMaterial(ColliderInterface *collider)
		{
			return create<BulletMaterial>(collider);
		}

		void BulletWorld::destroyMaterial(MaterialInterface *material)
		{
			destroy(static_cast<BulletMaterial *>(material));
		}

		bool BulletWorld::needBroadphaseCollision(btBroadphaseProxy *proxy1, btBroadphaseProxy *proxy2) const
		{
			const std::uint32_t shapeGroup1 = static_cast<std::uint32_t>(proxy1->m_collisionFilterGroup) & 31;
			const std::uint32_t shapeGroup2 = static_cast<std::uint32_t>(proxy2->m_collisionFilterGroup) & 31;
			return (groupCollisionFlags[shapeGroup1] & (1 << shapeGroup2)) != 0;
		}
	}
}