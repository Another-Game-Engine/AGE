#pragma once

#include "NullCapsuleCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullCapsuleCollider::NullCapsuleCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), CapsuleColliderInterface(world, data), NullCollider(world, data)
		{
			return;
		}

		// Inherited Methods
		void NullCapsuleCollider::setCenter(const glm::vec3 &center)
		{
			return;
		}

		glm::vec3 NullCapsuleCollider::getCenter(void) const
		{
			return glm::vec3();
		}

		void NullCapsuleCollider::setHeight(float height)
		{
			return;
		}

		float NullCapsuleCollider::getHeight(void) const
		{
			return 0.0f;
		}

		void NullCapsuleCollider::setRadius(float radius)
		{
			return;
		}

		float NullCapsuleCollider::getRadius(void) const
		{
			return 0.0f;
		}
	}
}