#include <Systems/FreeFlyCamera.hh>
#include <Components/CameraComponent.hpp>
#include <Core/Inputs/Input.hh>
#include <Context/IRenderContext.hh>
#include <Threads/ThreadManager.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Components/FreeFlyComponent.hh>

namespace AGE
{
	FreeFlyCamera::FreeFlyCamera(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene))
	{
		_name = "Free fly camera";
	}

	bool FreeFlyCamera::initialize()
	{
		_cameras.requireComponent<CameraComponent>();
		_cameras.requireComponent<FreeFlyComponent>();
		return (true);
	}

	void FreeFlyCamera::updateBegin(float time)
	{

	}

	void FreeFlyCamera::mainUpdate(float time)
	{
		const float verticalAngleLimit = glm::pi<float>();
		size_t camIndex = 0;

		if (_cameras.getCollection().size() < _cameraAngles.size())
			_cameraAngles.resize(_cameras.getCollection().size());
		for (auto cam : _cameras.getCollection())
		{
			FreeFlyComponent const *component = cam.getComponent<FreeFlyComponent>();
			auto &camLink = cam.getLink();

			if (camIndex == _cameraAngles.size())
			{
				glm::quat camRotation = camLink.getOrientation();
				_cameraAngles.emplace_back(glm::vec2(glm::eulerAngles(camRotation)));
			}

			if (component->handleKeyboard)
				_handleKeyboard(time, camLink, camIndex, component);
			if (component->handleMouse)
				_handleMouse(time, camLink, camIndex, component);
			if (component->handleController)
				_handleController(time, camLink, camIndex, component);

			_cameraAngles[camIndex].x = glm::clamp(_cameraAngles[camIndex].x, -verticalAngleLimit, verticalAngleLimit);

			glm::quat finalOrientation = glm::quat(glm::vec3(_cameraAngles[camIndex], 0));

			camLink.setOrientation(finalOrientation);
			++camIndex;
		}
	}

	void FreeFlyCamera::updateEnd(float time)
	{

	}

	void FreeFlyCamera::_handleKeyboard(float time, Link &camLink, size_t camIdx, FreeFlyComponent const *component)
	{
		float camTranslationSpeed = component->camTranslationSpeed;
		Input *inputs = _scene->getInstance<Input>();

		// If shift is pressed, we accelerate
		if (inputs->getPhysicalKeyPressed(component->accelerate))
			camTranslationSpeed += component->maxAcceleration;
		// translations
		if (inputs->getPhysicalKeyPressed(component->translateForward))
			camLink.setForward(glm::vec3(0, 0, -camTranslationSpeed * time));
		if (inputs->getPhysicalKeyPressed(component->translateBackward))
			camLink.setForward(glm::vec3(0, 0, camTranslationSpeed * time));
		if (inputs->getPhysicalKeyPressed(component->translateLeft))
			camLink.setForward(glm::vec3(-camTranslationSpeed * time, 0, 0));
		if (inputs->getPhysicalKeyPressed(component->translateRight))
			camLink.setForward(glm::vec3(camTranslationSpeed * time, 0, 0));
		// rotations
		if (inputs->getPhysicalKeyPressed(component->rotateUp))
			_cameraAngles[camIdx].x += component->camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(component->rotateDown))
			_cameraAngles[camIdx].x -= component->camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(component->rotateRight))
			_cameraAngles[camIdx].y -= component->camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(component->rotateLeft))
			_cameraAngles[camIdx].y += component->camRotationSpeed * time;
	}

	void FreeFlyCamera::_handleMouse(float time, Link &camLink, size_t camIdx, FreeFlyComponent const *component)
	{
		Input *inputs = _scene->getInstance<Input>();

		// On click, the context grab the mouse
		if (inputs->getMouseButtonJustPressed(AGE_MOUSE_RIGHT))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ContextGrabMouse>(true);
		}
		else if (inputs->getMouseButtonJustReleased(AGE_MOUSE_RIGHT))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ContextGrabMouse>(false);
		}
		// If clicked, handle the rotation with the mouse
		if (inputs->getMouseButtonPressed(AGE_MOUSE_RIGHT))
		{
			_cameraAngles[camIdx].y -= (float)inputs->getMouseDelta().x * component->camMouseRotationSpeed;
			_cameraAngles[camIdx].x -= (float)inputs->getMouseDelta().y * component->camMouseRotationSpeed;
		}
	}

	void FreeFlyCamera::_handleController(float time, Link &camLink, size_t camIdx, FreeFlyComponent const *component)
	{
		float camTranslationSpeed = component->camTranslationSpeed;
		Input *inputs = _scene->getInstance<Input>();
		Joystick controller;

		// Handle the Xbox controller
		if (inputs->getJoystick(0, controller))
		{
			float rightTrigger = controller.getAxis(AGE_JOYSTICK_AXIS_TRIGGERRIGHT) * 0.5f + 0.5f;

			// If right trigger pressed, accelerate
			camTranslationSpeed += rightTrigger * component->maxAcceleration;
			// Handle translations
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_LEFTY)) > 0.3)
				camLink.setForward(glm::vec3(0.f, 0.f, controller.getAxis(AGE_JOYSTICK_AXIS_LEFTY) * camTranslationSpeed * time));
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_LEFTX)) > 0.3)
				camLink.setForward(glm::vec3(controller.getAxis(AGE_JOYSTICK_AXIS_LEFTX) * camTranslationSpeed * time, 0.f, 0.f));
			// Handle rotations
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTX)) > 0.3)
				_cameraAngles[camIdx].y -= controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTX) * component->camRotationSpeed * time;
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTY)) > 0.3)
				_cameraAngles[camIdx].x -= controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTY) * component->camRotationSpeed * time;
		}
	}

}