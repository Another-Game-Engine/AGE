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
			: WorldInterface(static_cast<PhysicsInterface *>(physics))
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

		RigidBodyInterface *BulletWorld::createRigidBody(const glm::vec3 &position)
		{
			return static_cast<RigidBodyInterface *>(new BulletRigidBody(this, position));
		}

		ColliderInterface *BulletWorld::createCollider(ColliderType colliderType)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return new BulletBoxCollider(static_cast<WorldInterface *>(this));
				case ColliderType::Capsule:
					return new BulletCapsuleCollider(static_cast<WorldInterface *>(this));
				case ColliderType::Mesh:
					return new BulletMeshCollider(static_cast<WorldInterface *>(this));
				case ColliderType::Sphere:
					return new BulletSphereCollider(static_cast<WorldInterface *>(this));
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