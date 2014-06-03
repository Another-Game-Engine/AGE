#ifndef   __FP_CONTROLLER_HPP__
# define  __FP_CONTROLLER_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Context/SdlContext.hh"
#include <Physic/BulletDynamicManager.hpp>
#include <array>
#include <Core/AScene.hh>
#include <Utils/GlmSerialization.hpp>
#include <Entities/Entity.hh>

namespace Component
{
	ATTRIBUTE_ALIGNED16(struct) FPController : public ComponentBase<FPController>
	{
		enum CONTROLS
		{
			FORWARD = 0,
			BACKWARD,
			LEFT,
			RIGHT,
			JUMP,
			RUN
		};
		BT_DECLARE_ALIGNED_ALLOCATOR();
		FPController();
		virtual ~FPController();
		void init(const Entity &entity, std::weak_ptr<AScene> scene, short filterGroup = 1, short filterMask = -1);
		virtual void reset();
		void clear();
		btKinematicCharacterController &getController();
		btGhostObject &getGhost();
		btConvexShape &getShape();

		void setKey(CONTROLS k, unsigned int key);
		void resetControls();

		FPController(const FPController &o)
		{
		_entity = o._entity;
		yOrientation = o.yOrientation;
		forwardWalkSpeed = o.forwardWalkSpeed;
		backwardWalkSpeed = o.backwardWalkSpeed;
		forwardRunSpeed = o.forwardRunSpeed;
		backwardRunSpeed = o.backwardRunSpeed;
		sideWalkSpeed = o.sideWalkSpeed;
		sideRunSpeed = o.sideRunSpeed;
		rotateXSpeed = o.rotateXSpeed;
		rotateYSpeed = o.rotateYSpeed;
		jumpSpeed = o.jumpSpeed;
		jumpHeight = o.jumpHeight;
		canJump = o.canJump;
		canRun = o.canRun;
		justJump = o.justJump;
		justArriveOnFloor = o.justArriveOnFloor;
		wasOnGround = o.wasOnGround;
		keys = o.keys;
		controls = o.controls;

		_controller = o._controller;
		_ghost = o._ghost;
		_shape = o._shape;
		_manager = o._manager;
		}

		FPController& operator=(const FPController &o)
		{
		_entity = o._entity;
		yOrientation = o.yOrientation;
		forwardWalkSpeed = o.forwardWalkSpeed;
		backwardWalkSpeed = o.backwardWalkSpeed;
		forwardRunSpeed = o.forwardRunSpeed;
		backwardRunSpeed = o.backwardRunSpeed;
		sideWalkSpeed = o.sideWalkSpeed;
		sideRunSpeed = o.sideRunSpeed;
		rotateXSpeed = o.rotateXSpeed;
		rotateYSpeed = o.rotateYSpeed;
		jumpSpeed = o.jumpSpeed;
		jumpHeight = o.jumpHeight;
		canJump = o.canJump;
		canRun = o.canRun;
		justJump = o.justJump;
		justArriveOnFloor = o.justArriveOnFloor;
		wasOnGround = o.wasOnGround;
		keys = o.keys;
		controls = o.controls;

		_controller = o._controller;
		_ghost = o._ghost;
		_shape = o._shape;
		_manager = o._manager;
		return *this;
		}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new FPController();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
			ar(yOrientation
				, forwardWalkSpeed
				, backwardWalkSpeed
				, forwardRunSpeed
				, backwardRunSpeed
				, sideWalkSpeed
				, sideRunSpeed
				, rotateXSpeed
				, rotateYSpeed
				, jumpSpeed
				, jumpHeight
				, canJump
				, canRun
				, justJump
				, justArriveOnFloor
				, wasOnGround);
			glm::mat4 m = convertBulletTransformToGLM(_ghost->getWorldTransform());
			ar(m);
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(yOrientation
				, forwardWalkSpeed
				, backwardWalkSpeed
				, forwardRunSpeed
				, backwardRunSpeed
				, sideWalkSpeed
				, sideRunSpeed
				, rotateXSpeed
				, rotateYSpeed
				, jumpSpeed
				, jumpHeight
				, canJump
				, canRun
				, justJump
				, justArriveOnFloor
				, wasOnGround);
			glm::mat4 m;
			//@CESAR TODO
			//init();
			ar(m);
			_ghost->setWorldTransform(convertGLMTransformToBullet(m));
		}

		// !Serialization
		////
		//////

		Entity _entity;
		float yOrientation;
		float forwardWalkSpeed;
		float backwardWalkSpeed;
		float forwardRunSpeed;
		float backwardRunSpeed;
		float sideWalkSpeed;
		float sideRunSpeed;
		float rotateXSpeed;
		float rotateYSpeed;
		float jumpSpeed;
		float jumpHeight;
		bool canJump;
		bool canRun;
		bool justJump;
		bool justArriveOnFloor;
		bool wasOnGround;
		std::array<unsigned int, 6> keys;
		std::array<bool, 6> controls;

	private:
		btKinematicCharacterController *_controller;
		btPairCachingGhostObject *_ghost;
		btConvexShape *_shape;
		std::shared_ptr<BulletDynamicManager> _manager;
	};
}

#endif    //__FP_CONTROLLER_HPP__