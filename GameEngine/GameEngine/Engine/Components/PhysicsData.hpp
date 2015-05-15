#pragma once

#include <Components/Component.hh>
#include <Physics/GenericData.hpp>

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

			Physics::Private::GenericData *getData(void);

			const Physics::Private::GenericData *getData(void) const;

		private:
			// Attributes
			Physics::Private::GenericData data;
		};
	}
}