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
		PhysXCollider::PhysXCollider(WorldInterface *world, Private::GenericData *data, std::vector<physx::PxShape *> shapes)
			: ColliderInterface(world, data), shapes(std::move(shapes))
		{
			assert(!this->shapes.empty() && "Invalid shape");
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
			for (physx::PxShape *shape : this->shapes)
			{
				body->attachShape(*shape);
			}
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
				for (physx::PxShape *shape : shapes)
				{
					body->detachShape(*shape);
				}
			}
			shapes.clear();
		}

		// Methods
		std::vector<physx::PxShape *> &PhysXCollider::getShapes(void)
		{
			return shapes;
		}

		const std::vector<physx::PxShape *> &PhysXCollider::getShapes(void) const
		{
			return shapes;
		}

		void PhysXCollider::updateShape(std::vector<physx::PxShape *> newShape)
		{
			assert(!newShape.empty() && "Invalid shape");
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			for (physx::PxShape *shape : shapes)
			{
				body->detachShape(*shape);
			}
			shapes = std::move(newShape);
			for (physx::PxShape *shape : shapes)
			{
				body->attachShape(*shape);
			}
		}

		// Inherited Methods
		void PhysXCollider::setAsTrigger(bool mustBeATrigger)
		{
			for (physx::PxShape *shape : shapes)
			{
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !mustBeATrigger);
				shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, mustBeATrigger);
			}
		}

		bool PhysXCollider::isATrigger(void) const
		{
			return shapes.empty() ? true : shapes[0]->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE);
		}

		void PhysXCollider::setFilterGroup(FilterGroup group)
		{
			physx::PxFilterData filterData;
			filterData.word0 = static_cast<physx::PxU32>(group);
			for (physx::PxShape *shape : shapes)
			{
				shape->setSimulationFilterData(filterData);
				shape->setQueryFilterData(filterData);
			}
		}

		FilterGroup PhysXCollider::getFilterGroup(void) const
		{
			return shapes.empty() ? GetDefaultFilterGroup() : static_cast<FilterGroup>(shapes[0]->getSimulationFilterData().word0);
		}

		void PhysXCollider::setMaterial(const std::string &name)
		{
			MaterialInterface *newMaterial = getWorld()->createMaterial(name);
			static_cast<ColliderInterface *>(this)->setMaterial(newMaterial);
			physx::PxMaterial *concreteMaterial = static_cast<PhysXMaterial *>(newMaterial)->getMaterial();
			for (physx::PxShape *shape : shapes)
			{
				shape->setMaterials(&concreteMaterial, 1);
			}
		}
	}
}