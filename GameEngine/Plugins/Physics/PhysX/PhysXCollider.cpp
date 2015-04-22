#include <cmath>

#include "PhysXCollider.hpp"
#include "PhysXMaterial.hpp"
#include "PhysXWorld.hpp"
#include "PhysXRigidBody.hpp"

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
				assert(getData()->data != nullptr && "Impossible to create actor");
				physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
				static_cast<PhysXWorld *>(world)->getScene()->addActor(*body);
				body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
				body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
				body->setMass(0.0f);
			}
			else
			{
				rigidBody = static_cast<PhysXRigidBody *>(getDataAs<physx::PxRigidDynamic>()->userData);
			}
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			body->userData = this;
			body->attachShape(*shape);
			setAsTrigger(IsTriggerByDefault());
		}

		// Destructor
		PhysXCollider::~PhysXCollider(void)
		{
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			if (rigidBody == nullptr)
			{
				static_cast<PhysXWorld *>(getWorld())->getScene()->removeActor(*body);
				body->release();
				getData()->data = nullptr;
			}
			else
			{
				body->userData = nullptr;
				body->detachShape(*shape);
			}
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