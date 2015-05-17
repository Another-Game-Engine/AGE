#pragma once

#include "NullSphereCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullSphereCollider::NullSphereCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), SphereColliderInterface(world, data), NullCollider(world, data), center(GetDefaultCenter()), radius(GetDefaultRadius())
		{
			return;
		}

		// Inherited Methods
		void NullSphereCollider::setCenter(const glm::vec3 &center)
		{
			this->center = center;
		}

		glm::vec3 NullSphereCollider::getCenter(void) const
		{
			return center;
		}

		void NullSphereCollider::setRadius(float radius)
		{
			this->radius = radius;
		}

		float NullSphereCollider::getRadius(void) const
		{
			return radius;
		}

		void NullSphereCollider::scale(const glm::vec3 &scaling)
		{
			radius *= std::pow(std::abs(scaling.x * scaling.y * scaling.z), 1.0f / 3.0f);
		}
	}
}