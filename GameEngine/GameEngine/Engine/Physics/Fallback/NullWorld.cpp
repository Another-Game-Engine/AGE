#include "NullWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullWorld::NullWorld(NullPhysics *physics)
			: WorldInterface(physics)
		{
			return;
		}

		// Inherited Methods
		void NullWorld::setGravity(const glm::vec3 &gravity)
		{
			return;
		}

		glm::vec3 NullWorld::getGravity(void) const
		{
			return glm::vec3();
		}

		void NullWorld::simulate(float stepSize)
		{
			return;
		}

		RigidBodyInterface *NullWorld::createRigidBody(const glm::vec3 &position)
		{
			return static_cast<RigidBodyInterface *>(new NullRigidBody(this));
		}

		MaterialInterface *NullWorld::createMaterial(void)
		{
			return static_cast<MaterialInterface *>(new NullMaterial);
		}
	}
}