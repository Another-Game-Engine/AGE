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

		void PhysicsData::init()
		{
		}

		void PhysicsData::init(Entity entity)
		{
			data.entity = entity;
		}

		Physics::Private::GenericData *PhysicsData::getData(void)
		{
			return &data;
		}


		void PhysicsData::_copyFrom(const ComponentBase *model)
		{
			data = ((PhysicsData*)(model))->data;
		}

		const Physics::Private::GenericData *PhysicsData::getData(void) const
		{
			return &data;
		}
	}
}