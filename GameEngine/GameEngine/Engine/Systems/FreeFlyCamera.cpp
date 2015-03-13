#include <Systems/FreeFlyCamera.hh>
#include <Components/CameraComponent.hpp>
#include <Core/Inputs/Input.hh>
#include <Context/IRenderContext.hh>
#include <Threads/ThreadManager.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>

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
		return (true);
	}

	void FreeFlyCamera::updateBegin(double time)
	{

	}

	void FreeFlyCamera::mainUpdate(double time)
	{
		const float verticalAngleLimit = glm::pi<float>();
		size_t camIndex = 0;

		if (_cameras.getCollection().size() < _cameraAngles.size())
			_cameraAngles.resize(_cameras.getCollection().size());
		for (auto cam : _cameras.getCollection())
		{
			auto &camLink = cam.getLink();

			if (camIndex == _cameraAngles.size())
			{
				glm::quat camRotation = camLink.getOrientation();
				_cameraAngles.emplace_back(glm::vec2(glm::eulerAngles(camRotation)));
			}

			_handleKeyboard(time, camLink, camIndex);
			_handleMouse(time, camLink, camIndex);
			_handleController(time, camLink, camIndex);

			_cameraAngles[camIndex].x = glm::clamp(_cameraAngles[camIndex].x, -verticalAngleLimit, verticalAngleLimit);

			glm::quat finalOrientation = glm::quat(glm::vec3(_cameraAngles[camIndex], 0));

			camLink.setOrientation(finalOrientation);
			++camIndex;
		}
	}

	void FreeFlyCamera::updateEnd(double time)
	{

	}

	void FreeFlyCamera::_handleKeyboard(double time, Link &camLink, size_t camIdx)
	{
		float camTranslationSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float camRotationSpeed = 2.0f;
		Input *inputs = _scene->getInstance<Input>();

		// If shift is pressed, we accelerate
		if (inputs->getPhysicalKeyPressed(AGE_LSHIFT))
			camTranslationSpeed += maxAcceleration;
		// translations
		if (inputs->getPhysicalKeyPressed(AGE_w))
			camLink.setForward(glm::vec3(0, 0, -camTranslationSpeed * time));
		if (inputs->getPhysicalKeyPressed(AGE_s))
			camLink.setForward(glm::vec3(0, 0, camTranslationSpeed * time));
		if (inputs->getPhysicalKeyPressed(AGE_a))
			camLink.setForward(glm::vec3(-camTranslationSpeed * time, 0, 0));
		if (inputs->getPhysicalKeyPressed(AGE_d))
			camLink.setForward(glm::vec3(camTranslationSpeed * time, 0, 0));
		// rotations
		if (inputs->getPhysicalKeyPressed(AGE_UP))
			_cameraAngles[camIdx].x += camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(AGE_DOWN))
			_cameraAngles[camIdx].x -= camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(AGE_RIGHT))
			_cameraAngles[camIdx].y -= camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(AGE_LEFT))
			_cameraAngles[camIdx].y += camRotationSpeed * time;

	}

	void FreeFlyCamera::_handleMouse(double time, Link &camLink, size_t camIdx)
	{
		float camMouseRotationSpeed = 0.001f;
		Input *inputs = _scene->getInstance<Input>();

		// On click, the context grab the mouse
		if (inputs->getMouseButtonJustPressed(AGE_MOUSE_LEFT))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ContextGrabMouse>(true);
		}
		else if (inputs->getMouseButtonJustReleased(AGE_MOUSE_LEFT))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ContextGrabMouse>(false);
		}
		// If clicked, handle the rotation with the mouse
		if (inputs->getMouseButtonPressed(AGE_MOUSE_LEFT))
		{
			_cameraAngles[camIdx].y -= (float)inputs->getMouseDelta().x * camMouseRotationSpeed;
			_cameraAngles[camIdx].x -= (float)inputs->getMouseDelta().y * camMouseRotationSpeed;
		}
	}

	void FreeFlyCamera::_handleController(double time, Link &camLink, size_t camIdx)
	{
		float camTranslationSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float camRotationSpeed = 2.0f;
		Input *inputs = _scene->getInstance<Input>();
		Joystick controller;

		// Handle the Xbox controller
		if (inputs->getJoystick(0, controller))
		{
			float rightTrigger = controller.getAxis(AGE_JOYSTICK_AXIS_TRIGGERRIGHT) * 0.5f + 0.5f;

			// If right trigger pressed, accelerate
			camTranslationSpeed += rightTrigger * maxAcceleration;
			// Handle translations
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_LEFTY)) > 0.3)
				camLink.setForward(glm::vec3(0.f, 0.f, controller.getAxis(AGE_JOYSTICK_AXIS_LEFTY) * camTranslationSpeed * time));
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_LEFTX)) > 0.3)
				camLink.setForward(glm::vec3(controller.getAxis(AGE_JOYSTICK_AXIS_LEFTX) * camTranslationSpeed * time, 0.f, 0.f));
			// Handle rotations
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTX)) > 0.3)
				_cameraAngles[camIdx].y -= controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTX) * camRotationSpeed * time;
			if (glm::abs(controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTY)) > 0.3)
				_cameraAngles[camIdx].x -= controller.getAxis(AGE_JOYSTICK_AXIS_RIGHTY) * camRotationSpeed * time;
		}
	}

}