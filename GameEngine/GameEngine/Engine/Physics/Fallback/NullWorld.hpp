#pragma once

#include <Physics/WorldInterface.hpp>
#include <Physics/Fallback/NullPhysics.hpp>
#include <Physics/Fallback/NullMaterial.hpp>
#include <Physics/Fallback/NullCollider.hpp>
#include <Physics/Fallback/NullBoxCollider.hpp>
#include <Physics/Fallback/NullCapsuleCollider.hpp>
#include <Physics/Fallback/NullMeshCollider.hpp>
#include <Physics/Fallback/NullSphereCollider.hpp>
#include <Physics/Fallback/NullRigidBody.hpp>
#include <Physics/MemoryPoolHelper.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullWorld final : public WorldInterface, public MemoryPoolHelper<NullRigidBody, NullMaterial, NullBoxCollider, NullCapsuleCollider, NullMeshCollider, NullSphereCollider>
		{
		public:
			// Constructors
			NullWorld(void) = delete;

			NullWorld(NullPhysics *physics);

			NullWorld(const NullWorld &) = delete;

			// Assignment Operators
			NullWorld &operator=(const NullWorld &) = delete;

		private:
			// Destructor
			~NullWorld(void) = default;

			// Attributes
			glm::vec3 gravity;

			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(Private::GenericData *data) override final;

			void destroyRigidBody(RigidBodyInterface *rigidBody) override final;

			ColliderInterface *createCollider(ColliderType colliderType, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data) override final;

			void destroyCollider(ColliderInterface *collider) override final;

			MaterialInterface *createMaterial(ColliderInterface *collider) override final;

			void destroyMaterial(MaterialInterface *material) override final;
		};
	}
}
