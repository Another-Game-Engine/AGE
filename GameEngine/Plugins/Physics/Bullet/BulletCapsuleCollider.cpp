#pragma once

#include "BulletCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletCapsuleCollider::BulletCapsuleCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), CapsuleColliderInterface(world, data), BulletCollider(world, data)
		{
			return;
		}

		// Inherited Methods
		void BulletCapsuleCollider::setCenter(const glm::vec3 &center)
		{
			// TO_DO
		}

		glm::vec3 BulletCapsuleCollider::getCenter(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletCapsuleCollider::setHeight(float height)
		{
			// TO_DO
		}

		float BulletCapsuleCollider::getHeight(void) const
		{
			// TO_DO
			return 0.0f;
		}

		void BulletCapsuleCollider::setRadius(float radius)
		{
			// TO_DO
		}

		float BulletCapsuleCollider::getRadius(void) const
		{
			// TO_DO
			return 0.0f;
		}
	}
}