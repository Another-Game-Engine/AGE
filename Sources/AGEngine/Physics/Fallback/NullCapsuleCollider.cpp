#pragma once

#include "NullCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullCapsuleCollider::NullCapsuleCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), CapsuleColliderInterface(world, data), NullCollider(world, data), center(GetDefaultCenter()), height(GetDefaultHeight()), radius(GetDefaultRadius())
		{
			return;
		}

		// Inherited Methods
		void NullCapsuleCollider::setCenter(const glm::vec3 &center)
		{
			this->center = center;
		}

		glm::vec3 NullCapsuleCollider::getCenter(void) const
		{
			return center;
		}

		void NullCapsuleCollider::setHeight(float height)
		{
			this->height = height;
		}

		float NullCapsuleCollider::getHeight(void) const
		{
			return height;
		}

		void NullCapsuleCollider::setRadius(float radius)
		{
			this->radius = radius;
		}

		float NullCapsuleCollider::getRadius(void) const
		{
			return radius;
		}

		void NullCapsuleCollider::scale(const glm::vec3 &scaling)
		{
			height *= scaling.y;
			radius *= std::sqrt(scaling.x * scaling.z);
		}
	}
}