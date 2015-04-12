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
			virtual ~NullWorld(void) = default;

		private:
			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(const glm::vec3 &position) override final;

			MaterialInterface *createMaterial(void) override final;
		};
	}
}
