#include "NullCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullCollider::NullCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data)
		{
			return;
		}

		// Inherited Methods
		void NullCollider::setAsTrigger(bool mustBeATrigger)
		{
			return;
		}

		bool NullCollider::isATrigger(void) const
		{
			return false;
		}

		void NullCollider::setFilterGroup(FilterGroup group)
		{
			return;
		}

		FilterGroup NullCollider::getFilterGroup(void) const
		{
			return FilterGroup::GroupI;
		}
	}
}