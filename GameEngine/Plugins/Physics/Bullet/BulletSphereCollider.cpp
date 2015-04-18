#pragma once

#include "BulletSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletSphereCollider::BulletSphereCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), SphereColliderInterface(world, data), BulletCollider(world, data)
		{
			return;
		}

		// Inherited Methods
		void BulletSphereCollider::setCenter(const glm::vec3 &center)
		{
			// TO_DO
		}

		glm::vec3 BulletSphereCollider::getCenter(void) const
		{
			// TO_DO
			return glm::vec3();
		}

		void BulletSphereCollider::setRadius(float radius)
		{
			// TO_DO
		}

		float BulletSphereCollider::getRadius(void) const
		{
			// TO_DO
			return 0.0f;
		}
	}
}