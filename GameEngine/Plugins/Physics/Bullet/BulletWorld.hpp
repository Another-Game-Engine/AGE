#pragma once

#include "WorldInterface.hpp"
#include "BulletPhysics.hpp"
#include "BulletRigidBody.hpp"
#include "BulletMaterial.hpp"
#include "BulletBoxCollider.hpp"
#include "BulletCapsuleCollider.hpp"
#include "BulletMeshCollider.hpp"
#include "BulletSphereCollider.hpp"
#include "MemoryPoolHelper.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld final : public WorldInterface, public MemoryPoolHelper<BulletRigidBody, BulletMaterial, BulletBoxCollider, BulletCapsuleCollider, BulletMeshCollider, BulletSphereCollider>
		{
		public:
			// Constructors
			BulletWorld(void) = delete;

			BulletWorld(BulletPhysics *physics, const glm::vec3 &gravity);

			BulletWorld(const BulletWorld &) = delete;

			// Assignment Operators
			BulletWorld &operator=(const BulletWorld &) = delete;

			// Destructor
			virtual ~BulletWorld(void) = default;

		private:
			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(Private::GenericData *data) override final;

			void destroyRigidBody(RigidBodyInterface *rigidBody) override final;

			ColliderInterface *createCollider(ColliderType colliderType, Private::GenericData *data) override final;

			void destroyCollider(ColliderInterface *collider) override final;

			MaterialInterface *createMaterial(ColliderInterface *collider) override final;

			void destroyMaterial(MaterialInterface *material) override final;
		};
	}
}