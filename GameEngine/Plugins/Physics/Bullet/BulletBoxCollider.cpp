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
			// TO_DO
		}

		glm::vec3 BulletBoxCollider::getSize(void) const
		{
			// TO_DO
			return glm::vec3();
		}
	}
}