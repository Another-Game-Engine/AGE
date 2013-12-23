#ifndef   __KINE_CHAR_CONT_SYSTEM_HPP__
# define  __KINE_CHAR_CONT_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Component/KinematicCharacterController.hpp>
#include <Entities/Entity.hh>
#include "../BulletManager.hpp"
#include <Core/Engine.hh>
#include <Component/Collision.hpp>
#include "Context/SdlContext.hh"


class KinematicCharacterControllerSystem : public System
{
public:
	KinematicCharacterControllerSystem(Engine &engine) : System(engine)
		, _manager(engine.getInstance<BulletManager>())
	{}
	virtual ~KinematicCharacterControllerSystem(){}
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

		for (auto e : _collection)
		{
			glm::mat4 m;
			auto &ghost = e->getComponent<Component::KineCharacterController>()->getGhost();
			auto &controller = e->getComponent<Component::KineCharacterController>()->getController();
			btTransform trans = ghost.getWorldTransform();

			btVector3 forwardDir = trans.getBasis()[2];
			btVector3 upDir = trans.getBasis()[1];
			btVector3 sideDir = trans.getBasis()[0];
			forwardDir.normalize();
			upDir.normalize();
			sideDir.normalize();
			btVector3 walkDirection = btVector3(0.00001, 0.0, 0.0);
			btScalar walkVelocity = btScalar(1.1) * 4.0;
			btScalar walkSpeed = walkVelocity * 0.1;

			if (dir[3]) // left
			{
				btMatrix3x3 orn = ghost.getWorldTransform().getBasis();
				orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), -0.1));
				ghost.getWorldTransform().setBasis(orn);
			}

			if (dir[1]) // right
			{
				btMatrix3x3 orn = ghost.getWorldTransform().getBasis();
				orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0),0.1));
				ghost.getWorldTransform().setBasis(orn);
			}

			if (dir[0]) // forward
				walkDirection += forwardDir;

			if (dir[2]) // backward
				walkDirection -= forwardDir;

			controller.setWalkDirection(walkDirection * walkSpeed);

			if (dir[4]) // jump
				controller.jump();

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
		require<Component::KineCharacterController>();
	}
};

#endif    //__KINE_CHAR_CONT_SYSTEM_HPP__