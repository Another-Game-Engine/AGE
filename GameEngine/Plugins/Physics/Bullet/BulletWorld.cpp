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

		RigidBodyInterface *BulletWorld::createRigidBody(void *&data)
		{
			return new BulletRigidBody(this, data);
		}

		ColliderInterface *BulletWorld::createCollider(ColliderType colliderType, void *&data)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return new BulletBoxCollider(this, data);
				case ColliderType::Capsule:
					return new BulletCapsuleCollider(this, data);
				case ColliderType::Mesh:
					return new BulletMeshCollider(this, data);
				case ColliderType::Sphere:
					return new BulletSphereCollider(this, data);
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		MaterialInterface *BulletWorld::createMaterial(ColliderInterface *collider)
		{
			return new BulletMaterial(collider);
		}
	}
}