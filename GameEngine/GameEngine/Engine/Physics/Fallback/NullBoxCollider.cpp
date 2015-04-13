#pragma once

#include "NullBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullBoxCollider::NullBoxCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), NullCollider(world, data)
		{
			return;
		}

		// Inherited Methods
		void NullBoxCollider::setCenter(const glm::vec3 &center)
		{
			return;
		}

		glm::vec3 NullBoxCollider::getCenter(void) const
		{
			return glm::vec3();
		}

		void NullBoxCollider::setSize(const glm::vec3 &size)
		{
			return;
		}

		glm::vec3 NullBoxCollider::getSize(void) const
		{
			return glm::vec3();
		}
	}
}