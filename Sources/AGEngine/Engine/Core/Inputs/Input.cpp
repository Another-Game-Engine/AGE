
#include <Core/Inputs/Input.hh>
#include <Configuration.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>

#ifdef AGE_ENABLE_IMGUI
#include <Core/Age_Imgui.hpp>
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
		resetInputs();
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

	bool 	Input::getWindowInput(AgeWindowInputs input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (_windowInputs[int(input)]);
	}

	bool 	Input::getMouseButtonPressed(AgeMouseButtons input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_KEY_PRESSED(_mouseInputs[int(input)]));
	}

	bool 	Input::getMouseButtonJustPressed(AgeMouseButtons input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_KEY_JUST_PRESSED(_mouseInputs[int(input)]));
	}

	bool 	Input::getMouseButtonJustReleased(AgeMouseButtons input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_KEY_JUST_RELEASED(_mouseInputs[int(input)]));
	}

	bool 	Input::getPhysicalKeyPressed(AgeKeys input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_PHYSICAL_KEY_PRESSED(_keyInputs[(int)input]));
	}
	bool 	Input::getMappedKeyPressed(AgeKeys input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_KEY_PRESSED(_keyInputs[(int)input]));
	}

	bool 	Input::getPhysicalKeyJustPressed(AgeKeys input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_PHYSICAL_KEY_JUST_PRESSED(_keyInputs[(int)input]));
	}

	bool 	Input::getMappedKeyJustPressed(AgeKeys input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_KEY_JUST_PRESSED(_keyInputs[(int)input]));
	}

	bool 	Input::getPhysicalKeyJustReleased(AgeKeys input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_PHYSICAL_KEY_JUST_RELEASED(_keyInputs[(int)input]));
	}

	bool 	Input::getMappedKeyJustReleased(AgeKeys input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (AGE_KEY_JUST_RELEASED(_keyInputs[(int)input]));
	}

	bool	Input::getJoystick(uint32_t joyId)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		return (_joysticks[joyId]._connected);
	}

	bool	Input::getJoystick(uint32_t joyId, Joystick &joystickInfos)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (_joysticks[joyId]._connected)
		{
			joystickInfos = _joysticks[joyId];
			return (true);
		}
		return (false);
	}

	void	Input::frameUpdate()
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_mouseDelY = _mouseDelX = 0;
		for (int i = 0; i < int(AgeWindowInputs::AGE_WINDOW_INPUT_NUMBER); ++i)
		{
			_windowInputs[i] = false;
		}
		for (int i = 0; i < int(AgeMouseButtons::AGE_MOUSE_BUTTONS_NUMBER); ++i)
		{
			_mouseInputs[i] = AGE_UNSET_KEY_JUST_PRESSED(_mouseInputs[i]);
			_mouseInputs[i] = AGE_UNSET_KEY_JUST_RELEASED(_mouseInputs[i]);
		}
		for (int i = 0; AgeKeys(i) < AgeKeys::AGE_KEY_NUMBER; ++i)
		{
			_keyInputs[i] = AGE_UNSET_KEY_JUST_PRESSED(_keyInputs[i]);
			_keyInputs[i] = AGE_UNSET_KEY_JUST_RELEASED(_keyInputs[i]);
			_keyInputs[i] = AGE_UNSET_PHYSICAL_KEY_JUST_PRESSED(_keyInputs[i]);
			_keyInputs[i] = AGE_UNSET_PHYSICAL_KEY_JUST_RELEASED(_keyInputs[i]);
		}
		for (int i = 0; i < AGE_JOYSTICK_MAX_NUMBER; ++i)
		{
			if (_joysticks[i]._connected)
				_joysticks[i]._frameUpdate();
		}
		_mouseWheelX = _mouseWheelY = 0;
	}

	void	Input::resetInputs()
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		for (int i = 0; i < int(AgeWindowInputs::AGE_WINDOW_INPUT_NUMBER); ++i)
			_windowInputs[i] = false;
		for (int i = 0; i < int(AgeMouseButtons::AGE_MOUSE_BUTTONS_NUMBER); ++i)
			_mouseInputs[i] = 0;
		for (int i = 0; AgeKeys(i) < AgeKeys::AGE_KEY_NUMBER; ++i)
			_keyInputs[i] = 0;
	}

	void 	Input::mouseButtonPressed(AgeMouseButtons input)
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_mouseInputs[int(input)] = 0;
		_mouseInputs[int(input)] = AGE_SET_KEY_PRESSED(_mouseInputs[int(input)]);
		_mouseInputs[int(input)] = AGE_SET_KEY_JUST_PRESSED(_mouseInputs[int(input)]);
	}

	void 	Input::mouseButtonReleased(AgeMouseButtons input)
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_mouseInputs[int(input)] = 0;
		_mouseInputs[int(input)] = AGE_SET_KEY_JUST_RELEASED(_mouseInputs[int(input)]);
	}

	void	Input::addWindowInput(AgeWindowInputs input)
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_windowInputs[int(input)] = true;
	}

	void	Input::keyInputPressed(AgeKeys mappedInput, AgeKeys physicalInput)
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_keyInputs[int(mappedInput)] = AGE_RESET_MAPPED_KEY_STATE(_keyInputs[int(mappedInput)]);
		_keyInputs[int(mappedInput)] = AGE_SET_KEY_JUST_PRESSED(_keyInputs[int(mappedInput)]);
		_keyInputs[int(mappedInput)] = AGE_SET_KEY_PRESSED(_keyInputs[int(mappedInput)]);
		_keyInputs[int(physicalInput)] = AGE_RESET_PHYSICAL_KEY_STATE(_keyInputs[int(physicalInput)]);
		_keyInputs[int(physicalInput)] = AGE_SET_PHYSICAL_KEY_JUST_PRESSED(_keyInputs[int(physicalInput)]);
		_keyInputs[int(physicalInput)] = AGE_SET_PHYSICAL_KEY_PRESSED(_keyInputs[int(physicalInput)]);
#ifdef AGE_ENABLE_IMGUI
		TMQ::TaskManager::emplaceMainTask<ImGuiKeyEvent>(mappedInput, true);
#endif
	}

	void 	Input::keyInputReleased(AgeKeys mappedInput, AgeKeys physicalInput)
	{
		SCOPE_profile_cpu_function("Inputs");
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_keyInputs[int(mappedInput)] = AGE_RESET_MAPPED_KEY_STATE(_keyInputs[int(mappedInput)]);
		_keyInputs[int(mappedInput)] = AGE_SET_KEY_JUST_RELEASED(_keyInputs[int(mappedInput)]);
		_keyInputs[int(physicalInput)] = AGE_RESET_PHYSICAL_KEY_STATE(_keyInputs[int(physicalInput)]);
		_keyInputs[int(physicalInput)] = AGE_SET_PHYSICAL_KEY_JUST_RELEASED(_keyInputs[int(physicalInput)]);
#ifdef AGE_ENABLE_IMGUI
		TMQ::TaskManager::emplaceMainTask<ImGuiKeyEvent>(mappedInput, false);
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

		_joysticks[joyId]._connected = true;
	}

	void				Input::removeJoystick(uint32_t joyId)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		_joysticks[joyId]._disconnect();
	}

	void	Input::setJoystickAxis(uint32_t joyId, AgeJoystickAxis joyAxis, float value)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId]._connected);
		_joysticks[joyId]._axis[int(joyAxis)] = value;
	}

	void	Input::setJoystickTrackBall(uint32_t joyId, uint32_t trackBallIdx, glm::ivec2 const &pos)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId]._connected && trackBallIdx < AGE_JOYSTICK_TRACKBALL_MAX_NUMBER);
		_joysticks[joyId]._trackBalls[trackBallIdx] = pos;
	}

	void	Input::setJoystickHat(uint32_t joyId, uint32_t joyHat, AgeJoystickHatDirections dir)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId]._connected && joyHat < AGE_JOYSTICK_HAT_MAX_NUMBER);
		_joysticks[joyId]._hats[joyHat] = dir;
	}

	void	Input::joystickButtonPressed(uint32_t joyId, AgeJoystickButtons joyButton)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId]._connected);
		_joysticks[joyId]._setButtonPressed(joyButton);
	}

	void	Input::joystickButtonReleased(uint32_t joyId, AgeJoystickButtons joyButton)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		assert(_joysticks[joyId]._connected);
		_joysticks[joyId]._setButtonReleased(joyButton);
	}

	void Input::sendMouseStateToIMGUI()
	{
		SCOPE_profile_cpu_function("Inputs");
#ifdef AGE_ENABLE_IMGUI
		glm::ivec2 mousePosition;
		{
			std::lock_guard<AGE::SpinLock> lock(_mutex);
			mousePosition = glm::ivec2(_mousePosX, _mousePosY);
		}
		TMQ::TaskManager::emplaceMainTask<ImGuiMouseStateEvent>(mousePosition,
			getMouseButtonPressed(AgeMouseButtons::AGE_MOUSE_LEFT),
			getMouseButtonPressed(AgeMouseButtons::AGE_MOUSE_MIDDLE),
			getMouseButtonPressed(AgeMouseButtons::AGE_MOUSE_RIGHT),
			((float)getMouseWheel().y) / 10.0f);
#endif
	}

}