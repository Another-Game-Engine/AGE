#pragma once

#include <Core/Inputs/AgeInputs.hh>

#include <glm/glm.hpp>
#include <string>
#include <cstdint>

namespace AGE
{
	class Joystick
	{
		friend class Input;
	public:
		Joystick();
		Joystick(Joystick const &oth) = default;
		~Joystick() = default;

	public:
		bool isConnected() const;
		std::string getName() const;
		glm::ivec2 getTrackBall(uint32_t trackBallIdx) const;
		AgeJoystickHatDirections getHat(uint32_t hatIdx) const;
		float getAxis(AgeJoystickAxis axis) const;
		bool getButtonPressed(AgeJoystickButtons button) const;
		bool getButtonJustPressed(AgeJoystickButtons button) const;
		bool getButtonJustReleased(AgeJoystickButtons button) const;

	private:
		void _frameUpdate();
		void _disconnect();
		void _setButtonPressed(AgeJoystickButtons button);
		void _setButtonReleased(AgeJoystickButtons button);

	private:
		bool _connected;
		std::string _name;
		float _axis[AGE_JOYSTICK_AXIS_NUMBER];
		uint8_t _buttons[AGE_JOYSTICK_BUTTON_NUMBER];
		AgeJoystickHatDirections _hats[AGE_JOYSTICK_HAT_MAX_NUMBER];
		glm::ivec2 _trackBalls[AGE_JOYSTICK_TRACKBALL_MAX_NUMBER];
	};
}
