#pragma once

// TODO TO CLEAN
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

class btGhostObject;

namespace AGE
{
	namespace Component
	{
		ATTRIBUTE_ALIGNED16(struct) FPController : public ComponentBase < FPController >
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
			void init(AScene *scene, const Entity &entity, short filterGroup = 1, short filterMask = -1);
			virtual void reset(AScene *);
			void clear();
			btKinematicCharacterController &getController();
			btGhostObject &getGhost();
			btConvexShape &getShape();

			void setKey(CONTROLS k, unsigned int key);
			void resetControls();

			FPController(const FPController &o);
			FPController& operator=(const FPController &o);

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
			BulletDynamicManager *_manager;
		};
	}
}