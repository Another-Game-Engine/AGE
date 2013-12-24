#ifndef   __KINE_CHAR_CONT_SYSTEM_HPP__
# define  __KINE_CHAR_CONT_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Components/FPController.hpp>
#include <Entities/Entity.hh>
#include "../BulletManager.hpp"
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include "Context/SdlContext.hh"


class FPControllerSystem : public System
{
public:
	FPControllerSystem(Engine &engine) : System(engine)
		, _manager(engine.getInstance<BulletManager>())
	{}
	virtual ~FPControllerSystem(){}
private:
	BulletManager &_manager;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		Input			&inputs = _engine.getInstance<Input>();

		bool dir[5] = { false, false, false, false, false };
		bool reset = false;
		if (inputs.getKey(SDLK_w))
			dir[0] = true;
		if (inputs.getKey(SDLK_d))
			dir[1] = true;
		if (inputs.getKey(SDLK_s))
			dir[2] = true;
		if (inputs.getKey(SDLK_a))
			dir[3] = true;
		if (inputs.getKey(SDLK_SPACE))
			dir[4] = true;
		if (inputs.getKey(SDLK_r))
			reset = true;
		auto angle = glm::vec2(-(float)inputs.getMouseDelta().x, (float)inputs.getMouseDelta().y);
		for (auto e : _collection)
		{
			glm::mat4 m;
			auto &ghost = e->getComponent<Component::FPController>()->getGhost();
			auto &controller = e->getComponent<Component::FPController>()->getController();
			btTransform trans = ghost.getWorldTransform();

			btVector3 forwardDir = trans.getBasis()[2];
			btVector3 upDir = trans.getBasis()[1];
			btVector3 sideDir = trans.getBasis()[0];
			forwardDir.normalize();
			upDir.normalize();
			sideDir.normalize();
			btVector3 walkDirection = btVector3(0.000001, 0.0, 0.0);
			btScalar walkVelocity = btScalar(1.1) * 10.0;
			btScalar walkSpeed = walkVelocity * time;

			{
				btMatrix3x3 orn = ghost.getWorldTransform().getBasis();
				orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), angle.x / 300.0f));
				ghost.getWorldTransform().setBasis(orn);
			}

			if (dir[3]) // left
			{
				walkDirection += sideDir;
			}

			if (dir[1]) // right
			{
				walkDirection -= sideDir;
			}

			if (dir[0]) // forward
				walkDirection += forwardDir;

			if (dir[2]) // backward
				walkDirection -= forwardDir;

			controller.setWalkDirection(walkDirection * walkSpeed);

			if (dir[4]) // jump
				controller.jump();
			if (reset)
				controller.warp(btVector3(0, 8, 0));

			trans = ghost.getWorldTransform();
			glm::mat4 t = convertBulletTransformToGLM(trans);

			m = glm::translate(m, posFromMat4(t));
			glm::vec3 rot = -rotFromMat4(t, false);
			m = glm::rotate(m, rot.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, rot.y, glm::vec3(0, 1, 0));
			m = glm::rotate(m, rot.z, glm::vec3(0, 0, 1));
			glm::vec3 scale = scaleFromMat4(e->getLocalTransform());
			m = glm::scale(m, scale);
			e->setLocalTransform() = m;
		}
	}

	virtual void initialize()
	{
		require<Component::FPController>();
		SDL_SetRelativeMouseMode(SDL_bool(true));
	}
};

#endif    //__KINE_CHAR_CONT_SYSTEM_HPP__