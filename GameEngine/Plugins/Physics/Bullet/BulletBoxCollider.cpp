#pragma once

#include "BulletBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletBoxCollider::BulletBoxCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), BulletCollider(world, data)
		{
			return;
		}

		// Inherited Methods
		void BulletBoxCollider::setCenter(const glm::vec3 &center)
		{
			// TO_DO
		}

		glm::vec3 BulletBoxCollider::getCenter(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletBoxCollider::setSize(const glm::vec3 &size)
		{
			// TO_DO
		}

		glm::vec3 BulletBoxCollider::getSize(void) const
		{
			// TO_DO
			return glm::vec3();
		}
	}
}