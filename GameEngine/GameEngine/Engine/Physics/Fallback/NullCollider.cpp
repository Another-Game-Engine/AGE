#include "NullCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullCollider::NullCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data)
		{
			setAsTrigger(IsTriggerByDefault());
			setFilterGroup(GetDefaultFilterGroup());
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

		void NullCollider::setMaterial(const std::string &name)
		{
			static_cast<ColliderInterface *>(this)->setMaterial(getWorld()->createMaterial(name));
		}
	}
}