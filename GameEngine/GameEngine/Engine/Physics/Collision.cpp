#include <utility>

#include <Physics/Collision.hpp>

namespace AGE
{
	namespace Physics
	{
		// Constructors
		Collision::Collision(Collider *hitCollider, std::vector<Contact> contacts, ContactType contactType)
			: hitCollider(hitCollider), contacts(std::move(contacts)), contactType(contactType)
		{
			return;
		}

		Collision::Collision(Collision &&other)
			: hitCollider(other.hitCollider), contacts(std::move(other.contacts)), contactType(other.contactType)
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
				contactType = other.contactType;
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

		ContactType Collision::getContactType(void) const
		{
			return contactType;
		}
	}
}