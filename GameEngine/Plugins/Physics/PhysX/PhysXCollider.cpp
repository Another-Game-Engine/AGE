#include "PhysXCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXCollider::PhysXCollider(WorldInterface *world, Private::GenericData *data, physx::PxShape *shape)
			: ColliderInterface(world, data), shape(shape)
		{
			assert(shape != nullptr && "Invalid shape");
			if (getData()->data == nullptr)
			{
				getData()->data = static_cast<PhysXPhysics *>(world->getPhysics())->getPhysics()->createRigidDynamic(physx::PxTransform(physx::PxIdentity));
			}
			assert(getData()->data != nullptr && "Impossible to create actor");
			getDataAs<physx::PxRigidDynamic>()->attachShape(*shape);
		}

		// Destructor
		PhysXCollider::~PhysXCollider(void)
		{
			assert(getData() != nullptr && "Impossible to get actor");
			getDataAs<physx::PxRigidDynamic>()->detachShape(*shape);
			shape->release();
			shape = nullptr;
		}

		// Methods
		physx::PxShape *PhysXCollider::getShape(void)
		{
			return shape;
		}

		const physx::PxShape *PhysXCollider::getShape(void) const
		{
			return shape;
		}

		// Inherited Methods
		void PhysXCollider::setAsTrigger(bool mustBeATrigger)
		{
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !mustBeATrigger);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, mustBeATrigger);
		}

		bool PhysXCollider::isATrigger(void) const
		{
			return shape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE);
		}

		void PhysXCollider::setFilterGroup(FilterGroup group)
		{
			physx::PxFilterData filterData;
			filterData.word0 = static_cast<physx::PxU32>(group);
			shape->setSimulationFilterData(filterData);
		}

		FilterGroup PhysXCollider::getFilterGroup(void) const
		{
			return static_cast<FilterGroup>(shape->getSimulationFilterData().word0);
		}
	}
}