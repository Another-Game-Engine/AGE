#include "BulletRigidBody.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletRigidBody::BulletRigidBody(BulletWorld *world, const glm::vec3 &position)
			: RigidBodyInterface(static_cast<WorldInterface *>(world))
		{
			// TO_DO
		}

		// Inherited Methods
		void BulletRigidBody::setAngularDrag(float angularDrag)
		{
			// TO_DO
		}

		float BulletRigidBody::getAngularDrag(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getAngularVelocity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getCenterOfMass(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setLinearDrag(float linearDrag)
		{
			// TO_DO
		}

		float BulletRigidBody::getLinearDrag(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getLinearVelocity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setMass(float mass)
		{
			// TO_DO
		}

		float BulletRigidBody::getMass(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setMaxAngularVelocity(float maxAngularVelocity)
		{
			// TO_DO
		}

		float BulletRigidBody::getMaxAngularVelocity(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
		{
			// TO_DO
		}

		float BulletRigidBody::getMaxDepenetrationVelocity(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletRigidBody::setPosition(const glm::vec3 &position)
		{
			// TO_DO
		}

		glm::vec3 BulletRigidBody::getPosition(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletRigidBody::setRotation(const glm::quat &rotation)
		{
			// TO_DO
		}

		glm::quat BulletRigidBody::getRotation(void) const
		{
			// TO_DO
			return glm::quat();
		}
	}
}