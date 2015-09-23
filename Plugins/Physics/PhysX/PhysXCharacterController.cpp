#pragma once

#include "PhysXCharacterController.hpp"
#include "PhysXWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		physx::PxControllerManager *PhysXCharacterController::_manager = nullptr;

		PhysXCharacterController::PhysXCharacterController(WorldInterface *world) :
			CharacterControllerInterface(world)
		{
			if (_manager == nullptr)
			{
				_manager = PxCreateControllerManager(*static_cast<PhysXWorld*>(_world)->getScene());
//				_manager->setOverlapRecoveryModule(true);
			}
			physx::PxCapsuleControllerDesc desc;

			desc.stepOffset = 0.5f;
			desc.upDirection = physx::PxVec3(0, 1, 0);
			MaterialInterface *material = world->getMaterial(WorldInterface::GetDefaultMaterialName());
			desc.material = static_cast<PhysXMaterial*>(material)->getMaterial();
			desc.radius = 0.5f;
			desc.height = 2.0f;

			_controller = _manager->createController(desc);
		}

		PhysXCharacterController::~PhysXCharacterController()
		{
			_controller->release();
		}

		uint32_t PhysXCharacterController::move(glm::vec3 const &displacement, float minDist, float elapsedTime, FilterGroup group)
		{
			physx::PxFilterData filterData;
			filterData.word0 = static_cast<physx::PxU32>(group);

			physx::PxControllerFilters characterControllerFilters;
			characterControllerFilters.mFilterData = &filterData;

			physx::PxControllerCollisionFlags collisionFlags = _controller->move(
				physx::PxVec3(displacement.x, displacement.y, displacement.z),
				minDist,
				elapsedTime,
				characterControllerFilters
				);
			uint32_t collisions = NO_COLLISION;
			if (collisionFlags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES))
				collisions |= COLLISION_SIDES;
			if (collisionFlags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP))
				collisions |= COLLISION_UP;
			if (collisionFlags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
				collisions |= COLLISION_DOWN;
			return (collisions);
		}

		void PhysXCharacterController::setRadius(float radius)
		{
			static_cast<physx::PxCapsuleController*>(_controller)->setRadius(radius);
		}

		void PhysXCharacterController::setHeight(float height)
		{
			static_cast<physx::PxCapsuleController*>(_controller)->setHeight(height);
		}

		void PhysXCharacterController::resize(float height)
		{
			static_cast<physx::PxCapsuleController*>(_controller)->resize(height);
		}

		float PhysXCharacterController::getRadius() const
		{
			return (static_cast<physx::PxCapsuleController*>(_controller)->getRadius());
		}

		float PhysXCharacterController::getHeight() const
		{
			return (static_cast<physx::PxCapsuleController*>(_controller)->getHeight());
		}

		glm::vec3 PhysXCharacterController::getPosition() const
		{
			physx::PxExtendedVec3 pos = _controller->getPosition();

			return (glm::vec3(pos.x, pos.y, pos.z));
		}

		bool PhysXCharacterController::setPosition(glm::vec3 const &position)
		{
			return (_controller->setPosition(physx::PxExtendedVec3(position.x, position.y, position.z)));
		}

	}
}