#include <utility>

#include <Physics/Collision.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		Collision::Collision(Collider *hitCollider, std::vector<Contact> contacts, CollisionType CollisionType)
			: hitCollider(hitCollider), contacts(std::move(contacts)), collisionType(CollisionType)
		{
			return;
		}

		Collision::Collision(Collision &&other)
			: hitCollider(other.hitCollider), contacts(std::move(other.contacts)), collisionType(other.collisionType)
		{
			other.hitCollider = nullptr;
		}

		// Assignment Operators
		Collision &Collision::operator=(Collision &&other)
		{
			if (this != &other)
			{
				hitCollider = other.hitCollider;
				other.hitCollider = nullptr;
				contacts = std::move(other.contacts);
				collisionType = other.collisionType;
			}
			return *this;
		}

		// Methods
		Collider *Collision::getHitCollider(void)
		{
			return hitCollider;
		}

		const Collider *Collision::getHitCollider(void) const
		{
			return hitCollider;
		}

		const std::vector<Contact> &Collision::getContacts(void) const
		{
			return contacts;
		}

		CollisionType Collision::getCollisionType(void) const
		{
			return collisionType;
		}
	}
}