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
			setFilterGroup(GetDefaultFilterGroup());
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

		void PhysXCollider::updateShape(physx::PxShape *newShape)
		{
			assert(newShape != nullptr && "Invalid shape");
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			body->detachShape(*shape);
			body->attachShape(*newShape);
			shape = newShape;
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
			shape->setQueryFilterData(filterData);
		}

		FilterGroup PhysXCollider::getFilterGroup(void) const
		{
			return static_cast<FilterGroup>(shape->getSimulationFilterData().word0);
		}

		void PhysXCollider::setMaterial(const std::string &name)
		{
			MaterialInterface *newMaterial = getWorld()->createMaterial(name);
			static_cast<ColliderInterface *>(this)->setMaterial(newMaterial);
			physx::PxMaterial *concreteMaterial = static_cast<PhysXMaterial *>(newMaterial)->getMaterial();
			
			const physx::PxU16 nbm = shape->getNbMaterials();
			physx::PxMaterial *m[100];
			shape->getMaterials(m, 100);

			shape->setMaterials(&concreteMaterial, 1);

			const physx::PxU16 nnbm = shape->getNbMaterials();
		}
	}
}