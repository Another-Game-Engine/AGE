
#include <Core/Inputs/Input.hh>
#include <Configuration.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>

#ifdef USE_IMGUI
#include <Utils/Age_Imgui.hpp>
#endif

namespace AGE
{

	Input::Input() :
		_mousePosX(0),
		_mousePosY(0),
		_mouseDelX(0),
		_mouseDelY(0),
		_mouseWheelX(0),
		_mouseWheelY(0)
	{
	}

	void 	Input::addInput(AgeInputs input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_inputs[input] = true;
	}

	void 	Input::removeInput(AgeInputs input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_inputs[input] = false;
	}

	void	Input::keyInputPressed(AgeKeys mappedInput, AgeKeys hardwareInput)
	{
		_mutex.lock();
		_keyMappedInputs[mappedInput] = true;
		_keyPhysicalInputs[hardwareInput] = true;
		_mutex.unlock();
#ifdef USE_IMGUI
		GetMainThread()->getQueue()->emplaceTask<ImGuiKeyEvent>(mappedInput, true);
#endif
	}

	void 	Input::keyInputReleased(AgeKeys mappedInput, AgeKeys hardwareInput)
	{
		_mutex.lock();
		_keyMappedInputs[mappedInput] = false;
		_keyPhysicalInputs[hardwareInput] = false;
		_mutex.unlock();
#ifdef USE_IMGUI
		GetMainThread()->getQueue()->emplaceTask<ImGuiKeyEvent>(mappedInput, false);
#endif
	}

	void 				Input::setMousePosition(glm::ivec2 const &pos, glm::ivec2 const &rel)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_mouseDelX = rel.x;
		_mouseDelY = rel.y;
		_mousePosX = pos.x;
		_mousePosY = pos.y;
	}

	void				Input::setMouseWheel(glm::ivec2 const &delta)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_mouseWheelX = delta.x;
		_mouseWheelY = delta.y;
	}

	void				Input::addJoystick(std::string const &name, uint32_t joyId)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_joysticks[joyId].connected = true;
	}

	void				Input::removeJoystick(uint32_t joyId)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_joysticks[joyId].disconnect();
	}

	void	Input::setJoystickAxis(uint32_t joyId, AgeJoystickAxis joyAxis, float value)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId].connected);
		_joysticks[joyId].axis[joyAxis] = value;
	}

	void	Input::setJoystickTrackBall(uint32_t joyId, glm::ivec2 const &pos)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId].connected);
		_joysticks[joyId].trackBall = pos;
	}

	void	Input::setJoystickHat(uint32_t joyId, uint32_t joyHat, AgeJoystickHatDirections dir)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId].connected);
		_joysticks[joyId].hats[joyHat] = dir;
	}

	void	Input::joystickButtonPressed(uint32_t joyId, AgeJoystickButtons joyButton)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId].connected);
		_joysticks[joyId].buttons[joyButton] = true;
	}

	void	Input::joystickButtonReleased(uint32_t joyId, AgeJoystickButtons joyButton)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId].connected);
		_joysticks[joyId].buttons[joyButton] = false;
	}

	void Input::sendMouseStateToIMGUI()
	{
#ifdef USE_IMGUI
		glm::ivec2 mousePosition(_mousePosX, _mousePosY);
		GetMainThread()->getQueue()->emplaceTask<ImGuiMouseStateEvent>(mousePosition,
			getInput(AGE_MOUSE_LEFT),
			getInput(AGE_MOUSE_MIDDLE),
			getInput(AGE_MOUSE_RIGHT));
#endif
	}

	glm::ivec2   	    Input::getMouseWheel()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return glm::ivec2(_mouseWheelX, _mouseWheelY);
	}

	glm::ivec2      	Input::getMousePosition()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return glm::ivec2(_mousePosX, _mousePosY);
	}

	glm::ivec2       	Input::getMouseDelta()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return glm::ivec2(_mouseDelX, _mouseDelY);
	}

	bool 	Input::getInput(AgeInputs input, bool handled)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (handled)
			_inputs[input] = false;
		return (_inputs[input]);
	}

	bool 	Input::getPhysicalKey(AgeKeys input, bool handled)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (handled)
			_keyPhysicalInputs[input] = false;
		return (_keyPhysicalInputs[input]);
	}

	bool 	Input::getMappedKey(AgeKeys input, bool handled)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (handled)
			_keyMappedInputs[input] = false;
		return (_keyMappedInputs[input]);
	}

	bool	Input::getJoystick(uint32_t joyId)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (_joysticks[joyId].connected);
	}

	bool	Input::getJoystick(uint32_t joyId, Joystick &joystickInfos)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (_joysticks[joyId].connected)
		{
			joystickInfos = _joysticks[joyId];
			return (true);
		}
		else
			return (false);
	}
}