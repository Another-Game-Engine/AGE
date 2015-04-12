#include "NullWorld.hpp"
#include "NullBoxCollider.hpp"
#include "NullCapsuleCollider.hpp"
#include "NullMeshCollider.hpp"
#include "NullSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullWorld::NullWorld(NullPhysics *physics)
			: WorldInterface(physics)
		{
			return;
		}

		// Inherited Methods
		void NullWorld::setGravity(const glm::vec3 &gravity)
		{
			return;
		}

		glm::vec3 NullWorld::getGravity(void) const
		{
			return glm::vec3();
		}

		void NullWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			return;
		}

		void NullWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			return;
		}

		void NullWorld::simulate(float stepSize)
		{
			return;
		}

		RigidBodyInterface *NullWorld::createRigidBody(const glm::vec3 &position)
		{
			return static_cast<RigidBodyInterface *>(new NullRigidBody(this));
		}

		ColliderInterface *NullWorld::createCollider(ColliderType colliderType)
		{
			switch (colliderType)
			{
				case ColliderType::Box:
					return new NullBoxCollider(static_cast<WorldInterface *>(this));
				case ColliderType::Capsule:
					return new NullCapsuleCollider(static_cast<WorldInterface *>(this));
				case ColliderType::Mesh:
					return new NullMeshCollider(static_cast<WorldInterface *>(this));
				case ColliderType::Sphere:
					return new NullSphereCollider(static_cast<WorldInterface *>(this));
				default:
					assert(!"Never reached");
					return nullptr;
			}
		}

		MaterialInterface *NullWorld::createMaterial(ColliderInterface *collider)
		{
			return new NullMaterial(collider);
		}
	}
}