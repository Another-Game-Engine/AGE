#include <SystemsCore/FreeFlyCamera.hh>
#include <ComponentsCore/CameraComponent.hpp>
#include <Core/Inputs/Input.hh>
#include <Context/IRenderContext.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <ComponentsCore/FreeFlyComponent.hh>
#include <Core/AScene.hh>

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
		for (auto &cam : _cameras.getCollection())
		{
			auto &camLink = cam->getLink();

			if (camIndex == _cameraAngles.size())
			{
				glm::quat camRotation = camLink.getOrientation();
				_cameraAngles.emplace_back(glm::vec2(glm::eulerAngles(camRotation)));
			}

			bool moved = false;
			moved |= _handleKeyboard(time, camLink, camIndex);
			moved |= _handleMouse(time, camLink, camIndex);
			moved |= _handleController(time, camLink, camIndex);

			if (moved)
			{
				_cameraAngles[camIndex].x = glm::clamp(_cameraAngles[camIndex].x, -verticalAngleLimit, verticalAngleLimit);
				glm::quat finalOrientation = glm::quat(glm::vec3(_cameraAngles[camIndex], 0));
				camLink.setOrientation(finalOrientation);
			}
			++camIndex;
		}
	}

	void FreeFlyCamera::updateEnd(float time)
	{

	}

	bool FreeFlyCamera::_handleKeyboard(float time, Link &camLink, size_t camIdx)
	{
		float camTranslationSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float camRotationSpeed = 2.0f;
		Input *inputs = _scene->getInstance<Input>();
		bool moved = false;

		// If shift is pressed, we accelerate
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_LSHIFT))
		{
			camTranslationSpeed += maxAcceleration;
			moved = true;
		}
		// translations
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_w))
		{
			camLink.setForward(glm::vec3(0, 0, -camTranslationSpeed * time));
			moved = true;
		}
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_s))
		{
			camLink.setForward(glm::vec3(0, 0, camTranslationSpeed * time));
			moved = true;
		}
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_a))
		{
			camLink.setForward(glm::vec3(-camTranslationSpeed * time, 0, 0));
			moved = true;
		}
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_d))
		{
			camLink.setForward(glm::vec3(camTranslationSpeed * time, 0, 0));
			moved = true;
		}
		// rotations
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_UP))
		{
			_cameraAngles[camIdx].x += camRotationSpeed * time;
			moved = true;
		}
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_DOWN))
		{
			_cameraAngles[camIdx].x -= camRotationSpeed * time;
			moved = true;
		}
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_RIGHT))
		{
			_cameraAngles[camIdx].y -= camRotationSpeed * time;
			moved = true;
		}
		if (inputs->getPhysicalKeyPressed(AgeKeys::AGE_LEFT))
		{
			_cameraAngles[camIdx].y += camRotationSpeed * time;
			moved = true;
		}
		return moved;
	}

	bool FreeFlyCamera::_handleMouse(float time, Link &camLink, size_t camIdx)
	{
		float camMouseRotationSpeed = 0.0005f;
		Input *inputs = _scene->getInstance<Input>();
		bool moved = false;

		// On click, the context grab the mouse
		if (inputs->getMouseButtonJustPressed(AgeMouseButtons::AGE_MOUSE_RIGHT))
		{
			TMQ::TaskManager::emplaceRenderTask<Tasks::Render::ContextGrabMouse>(true);
		}
		else if (inputs->getMouseButtonJustReleased(AgeMouseButtons::AGE_MOUSE_RIGHT))
		{
			TMQ::TaskManager::emplaceRenderTask<Tasks::Render::ContextGrabMouse>(false);
		}
		// If clicked, handle the rotation with the mouse
		if (inputs->getMouseButtonPressed(AgeMouseButtons::AGE_MOUSE_RIGHT))
		{
			_cameraAngles[camIdx].y -= (float)inputs->getMouseDelta().x * camMouseRotationSpeed;
			_cameraAngles[camIdx].x -= (float)inputs->getMouseDelta().y * camMouseRotationSpeed;
			moved = true;
		}
		return moved;
	}

	bool FreeFlyCamera::_handleController(float time, Link &camLink, size_t camIdx)
	{
		float camTranslationSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float camRotationSpeed = 2.0f;
		Input *inputs = _scene->getInstance<Input>();
		Joystick controller;
		bool moved = false;

		// Handle the Xbox controller
		if (inputs->getJoystick(0, controller))
		{
			float rightTrigger = controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_TRIGGERRIGHT) * 0.5f + 0.5f;

			// If right trigger pressed, accelerate
			camTranslationSpeed += rightTrigger * maxAcceleration;
			// Handle translations
			if (glm::abs(controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_LEFTY)) > 0.3)
			{
				camLink.setForward(glm::vec3(0.f, 0.f, controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_LEFTY) * camTranslationSpeed * time));
				moved = true;
			}
			if (glm::abs(controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_LEFTX)) > 0.3)
			{
				camLink.setForward(glm::vec3(controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_LEFTX) * camTranslationSpeed * time, 0.f, 0.f));
				moved = true;
			}
			// Handle rotations
			if (glm::abs(controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_RIGHTX)) > 0.3)
			{
				_cameraAngles[camIdx].y -= controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_RIGHTX) * camRotationSpeed * time;
				moved = true;
			}
			if (glm::abs(controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_RIGHTY)) > 0.3)
			{
				_cameraAngles[camIdx].x -= controller.getAxis(AgeJoystickAxis::AGE_JOYSTICK_AXIS_RIGHTY) * camRotationSpeed * time;
				moved = true;
			}
		}
		return moved;
	}

}