#pragma once

#include "WorldInterface.hpp"
#include "BulletPhysics.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld final : public WorldInterface
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

			void simulate(float stepSize) override final;

			RigidBodyInterface *createRigidBody(const glm::vec3 &position) override final;

			MaterialInterface *createMaterial(void) override final;
		};
	}
}
