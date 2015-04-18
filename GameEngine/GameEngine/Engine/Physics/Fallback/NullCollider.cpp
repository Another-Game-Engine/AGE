#include "NullCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullCollider::NullCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data)
		{
			return;
		}

		// Inherited Methods
		void NullCollider::setAsTrigger(bool mustBeATrigger)
		{
			isTrigger = mustBeATrigger;
		}

		bool NullCollider::isATrigger(void) const
		{
			return isTrigger;
		}

		void NullCollider::setFilterGroup(FilterGroup group)
		{
			filterGroup = group;
		}

		FilterGroup NullCollider::getFilterGroup(void) const
		{
			return filterGroup;
		}
	}
}