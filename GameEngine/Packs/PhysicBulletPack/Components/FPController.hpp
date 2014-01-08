#ifndef   __FP_CONTROLLER_HPP__
# define  __FP_CONTROLLER_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Context/SdlContext.hh"
#include <Managers/BulletDynamicManager.hpp>
#include <array>

namespace Component
{
	ATTRIBUTE_ALIGNED16(class) FPController : public ComponentBase<FPController>
	{
	public:
		enum CONTROLS
		{
			FORWARD = 0,
			BACKWARD,
			LEFT,
			RIGHT,
			JUMP,
			RUN
		};
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();
		FPController(AScene *scene, Handle &entity);
		virtual ~FPController();
		void init();
		virtual void reset();
		void clear();
		btKinematicCharacterController &getController();
		btGhostObject &getGhost();
		btConvexShape &getShape();
		const float getYOrientation() const;
		void setYOrientation(float y);
		const float getForwardWalkSpeed() const;
		void setForwardWalkSpeed(float s);
		const float getForwardRunSpeed() const;
		void setForwardRunSpeed(float s);
		const float getBackwardWalkSpeed() const;
		void setBackwardWalkSpeed(float s);
		const float getBackwardRunSpeed() const;
		void setBackwardRunSpeed(float s);
		const float getSideWalkSpeed() const;
		void setSideWalkSpeed(float s);
		const float getSideRunSpeed() const;
		void setSideRunSpeed(float s);
		const float getRotateXSpeed() const;
		void setRotateXSpeed(float s);
		const float getRotateYSpeed() const;
		void setRotateYSpeed(float s);
		const float getJumpSpeed() const;
		void setJumpSpeed(float s);
		const float getJumpHeight() const;
		void setJumpHeight(float s);
		bool canJump() const;
		void setCanJump(bool b);
		bool canRun() const;
		void setCanRun(bool b);
		void setKey(CONTROLS k, unsigned int key);
		std::array<unsigned int, 6> &getKeys();
		std::array<bool, 6> &getControls();
		void resetControls();
	protected:
		btKinematicCharacterController *_controller;
		btPairCachingGhostObject *_ghost;
		btConvexShape *_shape;
		float _yOrientation;
		float _forwardWalkSpeed;
		float _backwardWalkSpeed;
		float _forwardRunSpeed;
		float _backwardRunSpeed;
		float _sideWalkSpeed;
		float _sideRunSpeed;
		float _rotateXSpeed;
		float _rotateYSpeed;
		float _jumpSpeed;
		float _jumpHeight;
		bool _canJump;
		bool _canRun;
		std::array<unsigned int, 6> _keys;
		std::array<bool, 6> _controls;
	};
}

#endif    //__FP_CONTROLLER_HPP__