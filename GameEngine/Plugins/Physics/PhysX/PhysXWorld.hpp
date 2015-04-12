#pragma once

#include "WorldInterface.hpp"
#include "PhysXPhysics.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXWorld final : public WorldInterface
		{
		public:
			// Constructors
			PhysXWorld(void) = delete;

			PhysXWorld(PhysXPhysics *physics, const glm::vec3 &gravity);

			PhysXWorld(const PhysXWorld &) = delete;

			// Assignment Operators
			PhysXWorld &operator=(const PhysXWorld &) = delete;

			// Destructor
			virtual ~PhysXWorld(void) = default;

			// Methods
			physx::PxScene *getScene(void);

			const physx::PxScene *getScene(void) const;

		private:
			// Attributes
			physx::PxScene *scene = nullptr;

			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(const glm::vec3 &position) override final;

			MaterialInterface *createMaterial(void) override final;
		};
	}
}
