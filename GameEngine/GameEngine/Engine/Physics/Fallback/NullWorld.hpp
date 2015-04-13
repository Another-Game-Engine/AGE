#pragma once

#include <Physics/WorldInterface.hpp>
#include <Physics/Fallback/NullPhysics.hpp>
#include <Physics/Fallback/NullMaterial.hpp>
#include <Physics/Fallback/NullRigidBody.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullWorld final : public WorldInterface
		{
		public:
			// Constructors
			NullWorld(void) = delete;

			NullWorld(NullPhysics *physics);

			NullWorld(const NullWorld &) = delete;

			// Assignment Operators
			NullWorld &operator=(const NullWorld &) = delete;

			// Destructor
			~NullWorld(void) = default;

		private:
			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(void *&data) override final;

			ColliderInterface *createCollider(ColliderType colliderType, void *&data) override final;

			MaterialInterface *createMaterial(ColliderInterface *collider) override final;
		};
	}
}
