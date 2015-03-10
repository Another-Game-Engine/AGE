#include <Core/Inputs/Joystick.hh>

namespace AGE
{
	Joystick::Joystick()
	{
		disconnect();
	}

	void Joystick::disconnect()
	{
		connected = false;
		name.clear();
		for (int i = 0; i < AGE_JOYSTICK_AXIS_NUMBER; ++i)
			axis[i] = 0;
		for (int i = 0; i < AGE_JOYSTICK_BUTTON_NUMBER; ++i)
			buttons[i] = false;
		for (int i = 0; i < AGE_JOYSTICK_HAT_MAX_NUMBER; ++i)
			hats[i] = AGE_JOYSTICK_HAT_CENTERED;
	}
}
