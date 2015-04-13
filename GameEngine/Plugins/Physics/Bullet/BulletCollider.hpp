#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletCollider : public virtual ColliderInterface
		{
		public:
			// Constructors
			BulletCollider(void) = delete;

			BulletCollider(WorldInterface *world, void *&data);

			BulletCollider(const BulletCollider &) = delete;

			// Assignment Operators
			BulletCollider &operator=(const BulletCollider &) = delete;

		protected:
			// Destructor
			virtual ~BulletCollider(void) = default;

		private:
			// Inherited Methods
			void setAsTrigger(bool mustBeATrigger) override final;

			bool isATrigger(void) const override final;

			void setFilterGroup(FilterGroup group) override final;

			FilterGroup getFilterGroup(void) const override final;
		};
	}
}