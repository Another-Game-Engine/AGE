#pragma once

#include "NullSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullSphereCollider::NullSphereCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), SphereColliderInterface(world, data), NullCollider(world, data)
		{
			return;
		}

		// Inherited Methods
		void NullSphereCollider::setCenter(const glm::vec3 &center)
		{
			return;
		}

		glm::vec3 NullSphereCollider::getCenter(void) const
		{
			return glm::vec3();
		}

		void NullSphereCollider::setRadius(float radius)
		{
			return;
		}

		float NullSphereCollider::getRadius(void) const
		{
			return 0.0f;
		}
	}
}