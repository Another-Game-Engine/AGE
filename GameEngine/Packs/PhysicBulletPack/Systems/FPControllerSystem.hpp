#ifndef   __KINE_CHAR_CONT_SYSTEM_HPP__
# define  __KINE_CHAR_CONT_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Components/FPController.hpp>
#include <Entities/Entity.hh>
#include <Managers/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>


class FPControllerSystem : public System
{
public:
	FPControllerSystem(Engine &engine) : System(engine)
		, _manager(&engine.getInstance<BulletCollisionManager>())
	{}
	virtual ~FPControllerSystem(){}
private:
	BulletCollisionManager *_manager;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _collection)
		{
			auto fp = e->getComponent<Component::FPController>();
			updateComponent(e, fp, time);
			auto &inputs = _engine.getInstance<Input>();
			auto &ghost = fp->getGhost();
			auto trans = ghost.getWorldTransform();

			glm::mat4 t = convertBulletTransformToGLM(trans);
			glm::mat4 m = glm::mat4(1);
			m = glm::translate(m, posFromMat4(t));
			glm::vec3 rot = -rotFromMat4(t, false);
			m = glm::rotate(m, rot.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, rot.y, glm::vec3(0, 1, 0));
			m = glm::rotate(m, rot.z, glm::vec3(0, 0, 1));
			glm::vec3 scale = scaleFromMat4(e->getLocalTransform());
			m = glm::scale(m, scale);
			e->setLocalTransform() = m;

			float yAngle = inputs.getMouseDelta().y;
			fp->setYOrientation(fp->getYOrientation() + yAngle * fp->getRotateYSpeed());
			if (fp->getYOrientation() >= 90.0f)
				fp->setYOrientation(89.9f);
			else if (fp->getYOrientation() <= -90.0f)
				fp->setYOrientation(-89.9f);

			e->setLocalTransform() = glm::rotate(e->getLocalTransform(), fp->getYOrientation(), glm::vec3(1, 0, 0));

		}
	}

	void updateComponent(Handle &entity, SmartPointer<Component::FPController> fp, double time)
	{
			fp->resetControls();
			auto &inputs = _engine.getInstance<Input>();
			auto &controls = fp->getControls();
			auto &keys = fp->getKeys();
			auto angle = glm::vec2((float)inputs.getMouseDelta().x, (float)inputs.getMouseDelta().y);

			// UPDATE KEYS
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls[i] = inputs.getKey(keys[i]);
			}

			auto &ghost = fp->getGhost();
			auto &controller = fp->getController();

			// UPDATE GHOST TRANSFORMATION
			btTransform trans = ghost.getWorldTransform();
			btVector3 forwardDir = trans.getBasis()[2];
			btVector3 upDir = trans.getBasis()[1];
			btVector3 sideDir = trans.getBasis()[0];
			forwardDir.normalize();
			upDir.normalize();
			sideDir.normalize();
			btVector3 walkDirection = btVector3(0.0, 0.0000000001, 0.0);
			bool isRunning = false;
			if (controls[Component::FPController::RUN])
				isRunning = true;
			if (controls[Component::FPController::LEFT])
				walkDirection += sideDir * (isRunning ? fp->getSideRunSpeed() : fp->getSideWalkSpeed()) * time;

			if (controls[Component::FPController::RIGHT])
				walkDirection -= sideDir  * (isRunning ? fp->getSideRunSpeed() : fp->getSideWalkSpeed()) * time;

			if (controls[Component::FPController::FORWARD])
				walkDirection += forwardDir * (isRunning ? fp->getForwardRunSpeed() : fp->getForwardWalkSpeed()) * time;

			if (controls[Component::FPController::BACKWARD])
				walkDirection -= forwardDir * (isRunning ? fp->getBackwardRunSpeed() : fp->getBackwardWalkSpeed()) * time;


			// HORIZONTAL ROTATION APPLIED ON GHOST
			{
				btMatrix3x3 orn = ghost.getWorldTransform().getBasis();
				orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), angle.x * fp->getRotateXSpeed()));
				ghost.getWorldTransform().setBasis(orn);
			}

			if (fp->canJump() && controls[Component::FPController::JUMP])
				controller.jump();

			controller.setWalkDirection(walkDirection);
	}

	virtual void initialize()
	{
		require<Component::FPController>();
		SDL_SetRelativeMouseMode(SDL_bool(true));
	}
};

#endif    //__KINE_CHAR_CONT_SYSTEM_HPP__