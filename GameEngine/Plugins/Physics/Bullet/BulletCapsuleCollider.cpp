#pragma once

#include "BulletCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletCapsuleCollider::BulletCapsuleCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), CapsuleColliderInterface(world, data), BulletCollider(world, data, new btCapsuleShape(GetDefaultRadius(), GetDefaultHeight())), center(GetDefaultCenter())
		{
			return;
		}

		// Inherited Methods
		void BulletCapsuleCollider::setCenter(const glm::vec3 &center)
		{
			this->center = center;
		}

		glm::vec3 BulletCapsuleCollider::getCenter(void) const
		{
			return center;
		}

		void BulletCapsuleCollider::setHeight(float height)
		{
			static_cast<btCapsuleShape *>(getShape())->setImplicitShapeDimensions(btVector3(getRadius(), 0.5f * height, getRadius()));
		}

		float BulletCapsuleCollider::getHeight(void) const
		{
			return 2.0f * static_cast<const btCapsuleShape *>(getShape())->getImplicitShapeDimensions().y();
		}

		void BulletCapsuleCollider::setRadius(float radius)
		{
			static_cast<btCapsuleShape *>(getShape())->setImplicitShapeDimensions(btVector3(radius, 0.5f * getHeight(), radius));
		}

		float BulletCapsuleCollider::getRadius(void) const
		{
			return static_cast<const btCapsuleShape *>(getShape())->getImplicitShapeDimensions().x();
		}
	}
}