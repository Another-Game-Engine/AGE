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

		void PhysicsData::_copyFrom(const ComponentBase *model)
		{
			assert(false); // not copyable
		}

		bool PhysicsData::doSerialize()
		{
			return false;
		}

		bool PhysicsData::serializeInExport()
		{
			return false;
		}
		
		bool PhysicsData::isExposedInEditor()
		{
			return false;
		}

	}
}