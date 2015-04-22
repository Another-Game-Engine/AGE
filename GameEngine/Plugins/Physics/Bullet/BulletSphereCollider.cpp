#pragma once

#include "BulletSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletSphereCollider::BulletSphereCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), SphereColliderInterface(world, data), BulletCollider(world, data, new btSphereShape(GetDefaultRadius())), center(GetDefaultCenter())
		{
			return;
		}

		// Inherited Methods
		void BulletSphereCollider::setCenter(const glm::vec3 &center)
		{
			assert(!"Not supported by Bullet");
			this->center = center;
		}

		glm::vec3 BulletSphereCollider::getCenter(void) const
		{
			assert(!"Not supported by Bullet");
			return center;
		}

		void BulletSphereCollider::setRadius(float radius)
		{
			static_cast<btSphereShape *>(getShape())->setUnscaledRadius(radius);
		}

		float BulletSphereCollider::getRadius(void) const
		{
			return static_cast<const btSphereShape *>(getShape())->getRadius();
		}
	}
}