#include <Core/Inputs/Joystick.hh>

namespace AGE
{
	Joystick::Joystick()
	{
		_disconnect();
	}

	bool Joystick::isConnected() const
	{
		return (_connected);
	}

	std::string Joystick::getName() const
	{
		return (_name);
	}

	glm::ivec2 Joystick::getTrackBall(uint32_t trackBallIdx) const
	{
		return (_trackBalls[trackBallIdx]);
	}

	AgeJoystickHatDirections Joystick::getHat(uint32_t hatIdx) const
	{
		return (_hats[hatIdx]);
	}

	float Joystick::getAxis(AgeJoystickAxis axis) const
	{
		return (_axis[axis]);
	}

	bool Joystick::getButtonPressed(AgeJoystickButtons button) const
	{
		return (AGE_KEY_PRESSED(_buttons[button]));
	}

	bool Joystick::getButtonJustPressed(AgeJoystickButtons button) const
	{
		return (AGE_KEY_JUST_PRESSED(_buttons[button]));
	}

	bool Joystick::getButtonJustReleased(AgeJoystickButtons button) const
	{
		return (AGE_KEY_JUST_RELEASED(_buttons[button]));
	}

	void Joystick::_frameUpdate()
	{
		for (int i = 0; i < AGE_JOYSTICK_BUTTON_NUMBER; ++i)
		{
			_buttons[i] = AGE_UNSET_KEY_JUST_PRESSED(_buttons[i]);
			_buttons[i] = AGE_UNSET_KEY_JUST_RELEASED(_buttons[i]);
		}
	}

	void Joystick::_disconnect()
	{
		_connected = false;
		_name.clear();
		for (int i = 0; i < AGE_JOYSTICK_AXIS_NUMBER; ++i)
			_axis[i] = 0;
		for (int i = 0; i < AGE_JOYSTICK_BUTTON_NUMBER; ++i)
			_buttons[i] = 0;
		for (int i = 0; i < AGE_JOYSTICK_HAT_MAX_NUMBER; ++i)
			_hats[i] = AGE_JOYSTICK_HAT_CENTERED;
	}

	void Joystick::_setButtonPressed(AgeJoystickButtons button)
	{
		_buttons[button] = 0;
		_buttons[button] = AGE_SET_KEY_JUST_PRESSED(_buttons[button]);
		_buttons[button] = AGE_SET_KEY_PRESSED(_buttons[button]);
	}

	void Joystick::_setButtonReleased(AgeJoystickButtons button)
	{
		_buttons[button] = 0;
		_buttons[button] = AGE_SET_KEY_JUST_RELEASED(_buttons[button]);
	}

}
