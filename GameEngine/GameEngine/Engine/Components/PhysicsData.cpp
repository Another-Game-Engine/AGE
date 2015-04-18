#include <Components/PhysicsData.hpp>

namespace AGE
{
	namespace Private
	{
		void PhysicsData::reset(void)
		{
			data = nullptr;
		}

		void PhysicsData::init(void)
		{
			return;
		}

		void *&PhysicsData::getData(void)
		{
			return data;
		}

		void * const &PhysicsData::getData(void) const
		{
			return data;
		}
	}
}