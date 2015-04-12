#pragma once

#include <Components/Component.hh>
#include <Physics/RigidBodyInterface.hpp>

namespace AGE
{
	class NewRigidBody final : public ComponentBase
	{
	public:
		// Constructors
		NewRigidBody(void) = default;

		NewRigidBody(const NewRigidBody &) = delete;

		// Assignment Operators
		NewRigidBody &operator=(const NewRigidBody &) = delete;

		// Destructor
		~NewRigidBody(void) = default;

		// Methods
		void init(void);

		void setAngularDrag(float angularDrag);

		float getAngularDrag(void) const;

		void setAngularVelocity(const glm::vec3 &angularVelocity);

		glm::vec3 getAngularVelocity(void) const;

		void setCenterOfMass(const glm::vec3 &centerOfMass);

		glm::vec3 getCenterOfMass(void) const;

		void setLinearDrag(float linearDrag);

		float getLinearDrag(void) const;

		void setLinearVelocity(const glm::vec3 &linearVelocity);

		glm::vec3 getLinearVelocity(void) const;

		void setMass(float mass);

		float getMass(void) const;

		void setMaxAngularVelocity(float maxAngularVelocity);

		float getMaxAngularVelocity(void) const;

		void setMaxDepenetrationVelocity(float maxDepenetrationVelocity);

		float getMaxDepenetrationVelocity(void) const;

		void setPosition(const glm::vec3 &position);

		glm::vec3 getPosition(void) const;

		void setRotation(const glm::quat &rotation);

		glm::quat getRotation(void) const;

		// Inherited Methods
		void reset(void);

	private:
		Physics::RigidBodyInterface *rigidBody = nullptr;
	};
}
