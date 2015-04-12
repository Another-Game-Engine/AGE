#pragma once

#include "RigidBodyInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld;

		class BulletRigidBody final : public RigidBodyInterface
		{
		public:
			// Constructors
			BulletRigidBody(void) = delete;

			BulletRigidBody(BulletWorld *world, const glm::vec3 &position);

			BulletRigidBody(const BulletRigidBody &) = delete;

			// Assignment Operators
			BulletRigidBody &operator=(const BulletRigidBody &) = delete;

			// Destructor
			~BulletRigidBody(void) = default;

		private:
			// Inherited Methods
			void setAngularDrag(float angularDrag) override final;

			float getAngularDrag(void) const override final;

			void setAngularVelocity(const glm::vec3 &angularVelocity) override final;

			glm::vec3 getAngularVelocity(void) const override final;

			void setCenterOfMass(const glm::vec3 &centerOfMass) override final;

			glm::vec3 getCenterOfMass(void) const override final;

			void setLinearDrag(float linearDrag) override final;

			float getLinearDrag(void) const override final;

			void setLinearVelocity(const glm::vec3 &linearVelocity) override final;

			glm::vec3 getLinearVelocity(void) const override final;

			void setMass(float mass) override final;

			float getMass(void) const override final;

			void setMaxAngularVelocity(float maxAngularVelocity) override final;

			float getMaxAngularVelocity(void) const override final;

			void setMaxDepenetrationVelocity(float maxDepenetrationVelocity) override final;

			float getMaxDepenetrationVelocity(void) const override final;

			void setPosition(const glm::vec3 &position) override final;

			glm::vec3 getPosition(void) const override final;

			void setRotation(const glm::quat &rotation) override final;

			glm::quat getRotation(void) const override final;
		};
	}
}