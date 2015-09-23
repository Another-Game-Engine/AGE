#pragma once

#include <vector>

#include <Physics/CollisionType.hpp>
#include <Physics/Contact.hpp>

namespace AGE
{
	class Collider;

	namespace Physics
	{
		class Collision final
		{
		public:
			// Constructors
			Collision(void) = delete;

			Collision(Collider *hitCollider, std::vector<Contact> contacts, CollisionType collisionType);

			Collision(const Collision &) = default;

			Collision(Collision &&other);

			// Assignment Operators
			Collision &operator=(const Collision &) = default;

			Collision &operator=(Collision &&other);

			// Destructor
			~Collision(void) = default;

			// Methods
			Collider *getHitCollider(void);

			const Collider *getHitCollider(void) const;

			const std::vector<Contact> &getContacts(void) const;

			CollisionType getCollisionType(void) const;

		private:
			// Attributes
			Collider *hitCollider = nullptr;

			std::vector<Contact> contacts;

			CollisionType collisionType;
		};
	}
}
