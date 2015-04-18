#pragma once

#include <Components/Component.hh>

namespace AGE
{
	namespace Private
	{
		class PhysicsData final : public ComponentBase
		{
		public:
			// Constructors
			PhysicsData(void) = default;

			PhysicsData(const PhysicsData &) = delete;

			// Assignment Operators
			PhysicsData &operator=(const PhysicsData &) = delete;

			// Destructor
			~PhysicsData(void) = default;

			// Inherited Methods
			void reset(void) override final;

			// Methods
			void init(void);

			void *&getData(void);

			void * const &getData(void) const;

		private:
			// Attributes
			void *data = nullptr;
		};
	}
}