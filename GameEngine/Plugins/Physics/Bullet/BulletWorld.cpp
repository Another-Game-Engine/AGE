#include <cassert>

#include "BulletWorld.hpp"
#include "BulletRigidBody.hpp"
#include "BulletMaterial.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletWorld::BulletWorld(BulletPhysics *physics, const glm::vec3 &gravity)
			: WorldInterface(static_cast<PhysicsInterface *>(physics))
		{
			// TO_DO
		}

		// Inherited Methods
		void BulletWorld::setGravity(const glm::vec3 &gravity)
		{
			// TO_DO
		}

		glm::vec3 BulletWorld::getGravity(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletWorld::enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			// TO_DO
		}

		void BulletWorld::disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2)
		{
			// TO_DO
		}

		void BulletWorld::simulate(float stepSize)
		{
			// TO_DO
		}

		RigidBodyInterface *BulletWorld::createRigidBody(const glm::vec3 &position)
		{
			return static_cast<RigidBodyInterface *>(new BulletRigidBody(this, position));
		}

		MaterialInterface *BulletWorld::createMaterial(void)
		{
			return static_cast<MaterialInterface *>(new BulletMaterial);
		}
	}
}