#include <Physics/Fallback/NullRigidBody.hpp>
#include <Physics/Fallback/NullWorld.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullRigidBody::NullRigidBody(NullWorld *world)
			: RigidBodyInterface(static_cast<WorldInterface *>(world))
		{
			return;
		}

		// Inherited Methods
		void NullRigidBody::setAngularDrag(float angularDrag)
		{
			return;
		}

		float NullRigidBody::getAngularDrag(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			return;
		}

		glm::vec3 NullRigidBody::getAngularVelocity(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			return;
		}

		glm::vec3 NullRigidBody::getCenterOfMass(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setLinearDrag(float linearDrag)
		{
			return;
		}

		float NullRigidBody::getLinearDrag(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			return;
		}

		glm::vec3 NullRigidBody::getLinearVelocity(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setMass(float mass)
		{
			return;
		}

		float NullRigidBody::getMass(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			return;
		}

		float NullRigidBody::getMaxAngularVelocity(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			return;
		}

		float NullRigidBody::getMaxDepenetrationVelocity(void) const
		{
			return 0.0f;
		}

		void NullRigidBody::setPosition(const glm::vec3 &position)
		{
			return;
		}

		glm::vec3 NullRigidBody::getPosition(void) const
		{
			return glm::vec3();
		}

		void NullRigidBody::setRotation(const glm::quat &rotation)
		{
			return;
		}

		glm::quat NullRigidBody::getRotation(void) const
		{
			return glm::quat();
		}
	}
}