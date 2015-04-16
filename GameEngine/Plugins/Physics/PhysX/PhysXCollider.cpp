#include <cmath>

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
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			if (body->userData == nullptr)
			{
				body->userData = static_cast<ColliderInterface *>(this);
			}
			body->attachShape(*shape);
		}

		// Destructor
		PhysXCollider::~PhysXCollider(void)
		{
			assert(getData() != nullptr && "Impossible to get actor");
			physx::PxRigidDynamic *body = getDataAs<physx::PxRigidDynamic>();
			body->userData = nullptr;
			body->detachShape(*shape);
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

		void PhysXCollider::scale(const glm::vec3 &scaling)
		{
			const physx::PxVec3 realScaling(scaling.x, scaling.y, scaling.z);
			physx::PxTransform localPose = shape->getLocalPose();
			switch (getColliderType())
			{
				case ColliderType::Box:
				{
					const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
					physx::PxVec3 &halfExtents = shape->getGeometry().box().halfExtents;
					halfExtents.x *= scalingMatrix.column0.magnitude();
					halfExtents.y *= scalingMatrix.column1.magnitude();
					halfExtents.z *= scalingMatrix.column2.magnitude();
					break;
				}
				case ColliderType::Capsule:
				{
					const physx::PxMat33 scalingMatrix = physx::PxMat33::createDiagonal(realScaling) * physx::PxMat33(localPose.q);
					physx::PxCapsuleGeometry &capsule = shape->getGeometry().capsule();
					capsule.halfHeight *= scalingMatrix.column0.magnitude();
					capsule.radius *= std::sqrt(scalingMatrix.column1.magnitude() * scalingMatrix.column2.magnitude());
					break;
				}
				case ColliderType::Mesh:
				{
					// TO_DO
					break;
				}
				case ColliderType::Sphere:
				{
					shape->getGeometry().sphere().radius *= std::pow(std::abs(realScaling.x * realScaling.y * realScaling.z), 1.0f / 3.0f);
					break;
				}
				default:
				{
					assert(!"Never reached");
					break;
				}
			}
			localPose.p = localPose.p.multiply(realScaling);
			shape->setLocalPose(localPose);
		}
	}
}