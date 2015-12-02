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

			CharacterControllerInterface *createCharacterController() override final;

			void destroyCharacterController(CharacterControllerInterface *cc) override final;
			virtual void fillDebugInformation(DebugDrawManager *debugDrawManager){}

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

			RaycasterInterface *createRaycaster(void) override final;

			RigidBodyInterface *createRigidBody(Private::GenericData *data) override final;

			void destroyRigidBody(RigidBodyInterface *rigidBody) override final;

			ColliderInterface *createCollider(ColliderType colliderType, const std::string &mesh, Private::GenericData *data) override final;

			void destroyCollider(ColliderInterface *collider) override final;

			MaterialInterface *createMaterial(const std::string &name) override final;

			void destroyMaterial(MaterialInterface *material) override final;
		};
	}
}
