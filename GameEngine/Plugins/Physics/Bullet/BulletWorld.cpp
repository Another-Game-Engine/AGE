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
			// TO_DO
		}

		// Inherited Methods
		void BulletWorld::setGravity(const glm::vec3 &gravity)
		{
			// TO_DO
		}

		glm::vec3 BulletWorld::getGravity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			// TO_DO
		}

		void BulletWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			// TO_DO
		}

		void BulletWorld::simulate(float stepSize)
		{
			// TO_DO
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
	}
}