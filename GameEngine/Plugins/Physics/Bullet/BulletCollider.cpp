#include "BulletCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletCollider::BulletCollider(WorldInterface *world, Private::GenericData *data)
			: ColliderInterface(world, data)
		{
			return;
		}

		// Inherited Methods
		void BulletCollider::setAsTrigger(bool mustBeATrigger)
		{
			// TO_DO
		}

		bool BulletCollider::isATrigger(void) const
		{
			// TO_DO
			return false;
		}

		void BulletCollider::setFilterGroup(FilterGroup group)
		{
			// TO_DO
		}

		FilterGroup BulletCollider::getFilterGroup(void) const
		{
			// TO_DO
			return FilterGroup::GroupI;
		}
	}
}