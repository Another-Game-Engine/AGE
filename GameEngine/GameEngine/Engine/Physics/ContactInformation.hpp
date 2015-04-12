#pragma once

#include <btBulletDynamicsCommon.h>

namespace AGE
{
	class ContactInformation final
	{
	public:
		// Constructors
		ContactInformation(void) = delete;

		ContactInformation(const btVector3 &contactPoint, const btVector3 &contactNormal);

		ContactInformation(const ContactInformation &) = default;

		// Assignment Operators
		ContactInformation &operator=(const ContactInformation &) = default;

		// Destructors
		~ContactInformation(void) = default;

		// Methods
		const btVector3 &getContactPoint(void) const;

		const btVector3 &getContactNormal(void) const;

	private:
		// Attributes
		btVector3 contactPoint;

		btVector3 contactNormal;
	};
}