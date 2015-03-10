#pragma once

#include <Core/Inputs/AgeInputs.hh>

#include <glm/glm.hpp>
#include <string>
#include <cstdint>

namespace AGE
{
	struct Joystick
	{
		bool connected;
		std::string name;

		float axis[AGE_JOYSTICK_AXIS_NUMBER];
		bool buttons[AGE_JOYSTICK_BUTTON_NUMBER];
		AgeJoystickHatDirections hats[AGE_JOYSTICK_HAT_MAX_NUMBER];
		glm::ivec2 trackBall;

		Joystick();
		void disconnect();
	};
}
