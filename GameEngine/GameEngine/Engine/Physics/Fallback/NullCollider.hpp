#pragma once

#include <Physics/ColliderInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullCollider : public virtual ColliderInterface
		{
		public:
			// Constructors
			NullCollider(void) = delete;

			NullCollider(WorldInterface *world, Private::GenericData *data);

			NullCollider(const NullCollider &) = delete;

			// Assignment Operators
			NullCollider &operator=(const NullCollider &) = delete;

		protected:
			// Destructor
			virtual ~NullCollider(void) = default;

		private:
			// Inherited Methods
			void setAsTrigger(bool mustBeATrigger) override final;

			bool isATrigger(void) const override final;

			void setFilterGroup(FilterGroup group) override final;

			FilterGroup getFilterGroup(void) const override final;

			void scale(const glm::vec3 &scaling) override final;
		};
	}
}