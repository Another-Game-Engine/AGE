#include <Components/PhysicsData.hpp>

namespace AGE
{
	namespace Private
	{
		void PhysicsData::reset(void)
		{
			data.data = nullptr;
		}

		void PhysicsData::init(void)
		{
			return;
		}

		Physics::Private::GenericData *PhysicsData::getData(void)
		{
			return &data;
		}

		const Physics::Private::GenericData *PhysicsData::getData(void) const
		{
			return &data;
		}
	}
}