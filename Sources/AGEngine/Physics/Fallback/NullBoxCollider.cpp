#pragma once

#include "NullBoxCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullBoxCollider::NullBoxCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data), BoxColliderInterface(world, data), NullCollider(world, data), center(GetDefaultCenter()), size(GetDefaultSize())
		{
			return;
		}

		// Inherited Methods
		void NullBoxCollider::setCenter(const glm::vec3 &center)
		{
			this->center = center;
		}

		glm::vec3 NullBoxCollider::getCenter(void) const
		{
			return center;
		}

		void NullBoxCollider::setSize(const glm::vec3 &size)
		{
			this->size = size;
		}

		glm::vec3 NullBoxCollider::getSize(void) const
		{
			return size;
		}

		void NullBoxCollider::scale(const glm::vec3 &scaling)
		{
			size *= scaling;
		}
	}
}