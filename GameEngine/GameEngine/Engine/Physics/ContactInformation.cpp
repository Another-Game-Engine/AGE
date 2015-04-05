#include "Physics/ContactInformation.hpp"

namespace AGE
{
	// Constructors
	ContactInformation::ContactInformation(const btVector3 &contactPoint, const btVector3 &contactNormal)
		: contactPoint(contactPoint), contactNormal(contactNormal)
	{
		return;
	}

	// Methods
	const btVector3 &ContactInformation::getContactPoint(void) const
	{
		return contactPoint;
	}

	const btVector3 &ContactInformation::getContactNormal(void) const
	{
		return contactNormal;
	}
}