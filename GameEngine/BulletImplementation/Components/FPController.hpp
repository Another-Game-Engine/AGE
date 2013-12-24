#ifndef   __FP_CONTROLLER_HPP__
# define  __FP_CONTROLLER_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "RigidBody.hpp"
#include "../BulletManager.hpp"

namespace Component
{
	ATTRIBUTE_ALIGNED16(class) FPController : public ComponentBase<KineCharacterController>
	{
	private:
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
		FPController(Engine &engine, Handle &entity);
		virtual ~FPController();
		void init();
		virtual void reset();
		void clear();
		btKinematicCharacterController &getController();
		btGhostObject &getGhost();
		btConvexShape &getShape();
		const glm::vec3 &getOrientation() const;
		void setOrientation(const glm::vec3 &v);
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
		void setJumpSpeed();
		const float getJumpHeight() const;
		void setJumpHeight(float s);
		bool canJump() const;
		void setCanJump(bool b);
		bool canRun() const;
		void setCanRun(bool b);
		void setKeys(unsigned int left
			, unsigned int right
			, unsigned int forward
			, unsigned int backward
			, unsigned int jump
			, unsigned int run);
	private:
		btKinematicCharacterController *_controller;
		btPairCachingGhostObject *_ghost;
		btConvexShape *_shape;
		glm::vec3 _orientation;
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
		unsigned int _leftKey;
		unsigned int _rightKey;
		unsigned int _forwarKey;
		unsigned int _backWardKey;
		unsigned int _jumpKey;
		unsigned int _runKey;
		bool _controls[6] = { false, false, false, false, false, false};
	};
}

#endif    //__FP_CONTROLLER_HPP__