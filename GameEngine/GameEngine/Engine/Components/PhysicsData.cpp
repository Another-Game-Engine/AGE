#include <Components/PhysicsData.hpp>

namespace AGE
{
	namespace Private
	{
		void PhysicsData::reset(void)
		{
			data.data = nullptr;
			data.entity = Entity();
		}

		void PhysicsData::init(Entity entity)
		{
			data.entity = entity;
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