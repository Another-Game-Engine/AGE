#include <Core/Inputs/Joystick.hh>

namespace AGE
{
	Joystick::Joystick()
	{
		_disconnect();
	}

	void Joystick::_frameUpdate()
	{
		for (int i = 0; i < int(AgeJoystickButtons::AGE_JOYSTICK_BUTTON_NUMBER); ++i)
		{
			_buttons[i] = AGE_UNSET_KEY_JUST_PRESSED(_buttons[i]);
			_buttons[i] = AGE_UNSET_KEY_JUST_RELEASED(_buttons[i]);
		}
	}

	void Joystick::_disconnect()
	{
		_connected = false;
		_name.clear();
		for (int i = 0; i < int(AgeJoystickAxis::AGE_JOYSTICK_AXIS_NUMBER); ++i)
			_axis[i] = 0;
		for (int i = 0; i < int(AgeJoystickButtons::AGE_JOYSTICK_BUTTON_NUMBER); ++i)
			_buttons[i] = 0;
		for (int i = 0; i < AGE_JOYSTICK_HAT_MAX_NUMBER; ++i)
			_hats[i] = AgeJoystickHatDirections::AGE_JOYSTICK_HAT_CENTERED;
	}

	void Joystick::_setButtonPressed(AgeJoystickButtons button)
	{
		_buttons[int(button)] = 0;
		_buttons[int(button)] = AGE_SET_KEY_JUST_PRESSED(_buttons[int(button)]);
		_buttons[int(button)] = AGE_SET_KEY_PRESSED(_buttons[int(button)]);
	}

	void Joystick::_setButtonReleased(AgeJoystickButtons button)
	{
		_buttons[int(button)] = 0;
		_buttons[int(button)] = AGE_SET_KEY_JUST_RELEASED(_buttons[int(button)]);
	}

}
