#pragma once

#include "BulletBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletBoxCollider::BulletBoxCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), BulletCollider(world, data, new btBoxShape(btVector3(GetDefaultSize().x / 2.0f, GetDefaultSize().y / 2.0f, GetDefaultSize().z / 2.0f))), center(GetDefaultCenter())
		{
			return;
		}

		// Inherited Methods
		void BulletBoxCollider::setCenter(const glm::vec3 &center)
		{
			assert(!"Not supported by Bullet");
			this->center = center;
		}

		glm::vec3 BulletBoxCollider::getCenter(void) const
		{
			assert(!"Not supported by Bullet");
			return center;
		}

		void BulletBoxCollider::setSize(const glm::vec3 &size)
		{
			btBoxShape *shape = static_cast<btBoxShape *>(getShape());
			const btVector3 halfExtents(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f);
			shape->setSafeMargin(halfExtents);
			const btVector3 margin(shape->getMargin(), shape->getMargin(), shape->getMargin());
			shape->setImplicitShapeDimensions((halfExtents * shape->getLocalScaling()) - margin);
		}

		glm::vec3 BulletBoxCollider::getSize(void) const
		{
			const btVector3 &size = static_cast<const btBoxShape *>(getShape())->getImplicitShapeDimensions();
			return glm::vec3(size.x(), size.y(), size.z());
		}
	}
}