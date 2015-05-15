#pragma once

#include "Contact.hpp"
#include "CollisionType.hpp"

namespace AGE
{
	class Collider;

	namespace Physics
	{
		class CollisionListener
		{
		public:
			// Constructors
			CollisionListener(void) = default;

			CollisionListener(const CollisionListener &) = default;

			// Assignment Operators
			CollisionListener &operator=(const CollisionListener &) = default;

			// Destructors
			virtual ~CollisionListener(void) = default;

			// Virtual Methods
			virtual void onCollision(Collider *currentCollider, Collider *hitCollider, const std::vector<Contact> &contacts, CollisionType CollisionType) = 0;
		};
	}
}