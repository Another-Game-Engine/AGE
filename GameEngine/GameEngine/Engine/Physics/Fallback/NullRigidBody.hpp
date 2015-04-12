#pragma once

#include <Physics/RigidBodyInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullWorld;

		class NullRigidBody final : public RigidBodyInterface
		{
		public:
			// Constructors
			NullRigidBody(void) = delete;

			NullRigidBody(NullWorld *world);

			NullRigidBody(const NullRigidBody &) = delete;

			// Assignment Operators
			NullRigidBody &operator=(const NullRigidBody &) = delete;

			// Destructor
			~NullRigidBody(void) = default;

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