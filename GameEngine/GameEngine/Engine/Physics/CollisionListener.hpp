#pragma once

#include "Contact.hpp"
#include "ContactType.hpp"

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
			virtual void onCollision(Collider *currentCollider, Collider *hitCollider, std::vector<Contact> contacts, ContactType contactType) = 0;
		};
	}
}