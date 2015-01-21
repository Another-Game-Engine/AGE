#include "FPControllerSystem.hpp"
#include <glm/glm.hpp>
#include <Core/AScene.hh>
#include <Core/Input.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Physic/Utils/BtConversion.hpp>

namespace AGE
{
	FPControllerSystem::FPControllerSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _manager(scene.lock()->getInstance<BulletCollisionManager>())
		, _filter(std::move(scene))
	{
		_name = "FP_controller_system";
	}
	FPControllerSystem::~FPControllerSystem(){}

	void FPControllerSystem::updateBegin(double time)
	{
	}

	void FPControllerSystem::updateEnd(double time)
	{}

	void FPControllerSystem::mainUpdate(double time)
	{
		//auto scene = _scene.lock();
		//for (auto e : _filter.getCollection())
		//{
		//	auto &entityTrans = scene->getLink(e)->getTransform();
		//	auto fp = scene->getComponent<Component::FPController>(e);
		//	updateComponent(e, fp, time);
		//	auto inputs = scene->getInstance<Input>();
		//	auto &ghost = fp->getGhost();
		//	auto trans = ghost.getWorldTransform();

		//	glm::mat4 t = convertBulletTransformToGLM(trans);
		//	glm::mat4 m = glm::mat4(1);
		//	m = glm::translate(m, posFromMat4(t));
		//	glm::vec3 rot = -rotFromMat4(t, false);
		//	m = glm::rotate(m, rot.x, glm::vec3(1, 0, 0));
		//	m = glm::rotate(m, rot.y, glm::vec3(0, 1, 0));
		//	m = glm::rotate(m, rot.z, glm::vec3(0, 0, 1));
		//	glm::vec3 scale = scaleFromMat4(entityTrans);
		//	m = glm::scale(m, scale);
		//	scene->setTransform(e, m, true);

		//	float yAngle = inputs->getMouseDelta().y;
		//	fp->yOrientation = fp->yOrientation + yAngle * fp->rotateYSpeed;
		//	if (fp->yOrientation >= 90.0f)
		//		fp->yOrientation = 89.9f;
		//	else if (fp->yOrientation <= -90.0f)
		//		fp->yOrientation = -89.9f;

		//	scene->setTransform(e, glm::rotate(m, fp->yOrientation, glm::vec3(1, 0, 0)), true);
		//}
	}

	void FPControllerSystem::updateComponent(Entity &entity, Component::FPController *fp, double time)
	{
		//float ftime = static_cast<float>(time);
		//fp->resetControls();
		//auto scene = _scene.lock();
		//auto inputs = scene->getInstance<Input>();
		//auto &controls = fp->controls;
		//auto &keys = fp->keys;
		//auto angle = glm::vec2((float)inputs->getMouseDelta().x, (float)inputs->getMouseDelta().y);

		//// UPDATE KEYS
		//for (std::size_t i = 0; i < controls.size(); ++i)
		//{
		//	controls[i] = inputs->getKey(keys[i]);
		//}

		//auto &ghost = fp->getGhost();
		//auto &controller = fp->getController();

		//// UPDATE GHOST TRANSFORMATION
		//btTransform trans = ghost.getWorldTransform();
		//btVector3 forwardDir = trans.getBasis()[2];
		//btVector3 upDir = trans.getBasis()[1];
		//btVector3 sideDir = trans.getBasis()[0];
		//forwardDir.normalize();
		//upDir.normalize();
		//sideDir.normalize();
		//btVector3 walkDirection = btVector3(0.0, 0.0000001, 0.0);
		//bool isRunning = false;
		//if (controls[Component::FPController::RUN])
		//	isRunning = true;
		//if (controls[Component::FPController::LEFT])
		//	walkDirection += sideDir * (isRunning ? fp->sideRunSpeed : fp->sideWalkSpeed);

		//if (controls[Component::FPController::RIGHT])
		//	walkDirection -= sideDir  * (isRunning ? fp->sideRunSpeed : fp->sideWalkSpeed);

		//if (controls[Component::FPController::FORWARD])
		//	walkDirection += forwardDir * (isRunning ? fp->forwardRunSpeed : fp->forwardWalkSpeed);

		//if (controls[Component::FPController::BACKWARD])
		//	walkDirection -= forwardDir * (isRunning ? fp->backwardRunSpeed : fp->backwardWalkSpeed);


		//// HORIZONTAL ROTATION APPLIED ON GHOST
		//{
		//	btMatrix3x3 orn = ghost.getWorldTransform().getBasis();
		//	orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), angle.x * fp->rotateXSpeed));
		//	ghost.getWorldTransform().setBasis(orn);
		//}

		//fp->justJump = false;
		//fp->justArriveOnFloor = false;
		//if (!fp->wasOnGround && controller.onGround())
		//	fp->justArriveOnFloor = true;
		//if (fp->canJump && controls[Component::FPController::JUMP] && fp->getController().onGround())
		//{
		//	controller.jump();
		//	fp->justJump = true;
		//}
		//fp->wasOnGround = controller.onGround();
		//controller.setWalkDirection(walkDirection);
	}

	bool FPControllerSystem::initialize()
	{
		//_filter.requireComponent<Component::FPController>();
		//SDL_SetRelativeMouseMode(SDL_bool(true));
		return true;
	}
}