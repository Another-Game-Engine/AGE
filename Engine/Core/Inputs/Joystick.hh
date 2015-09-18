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
		inline bool isConnected() const { return (_connected); }
		inline std::string getName() const { return (_name); }
		inline glm::ivec2 getTrackBall(uint32_t trackBallIdx) const { return (_trackBalls[trackBallIdx]); }
		inline AgeJoystickHatDirections getHat(uint32_t hatIdx) const { return (_hats[hatIdx]); }
		inline float getAxis(AgeJoystickAxis axis) const
		{
			return (_axis[int(axis)]);
		}
		inline bool getButtonPressed(AgeJoystickButtons button) const { return (AGE_KEY_PRESSED(_buttons[int(button)])); }
		inline bool getButtonJustPressed(AgeJoystickButtons button) const { return (AGE_KEY_JUST_PRESSED(_buttons[int(button)])); }
		inline bool getButtonJustReleased(AgeJoystickButtons button) const { return (AGE_KEY_JUST_RELEASED(_buttons[int(button)])); }

	private:
		void _frameUpdate();
		void _disconnect();
		void _setButtonPressed(AgeJoystickButtons button);
		void _setButtonReleased(AgeJoystickButtons button);

	private:
		bool _connected;
		std::string _name;
		float _axis[AgeJoystickAxis::AGE_JOYSTICK_AXIS_NUMBER];
		uint8_t _buttons[AgeJoystickButtons::AGE_JOYSTICK_BUTTON_NUMBER];
		AgeJoystickHatDirections _hats[AGE_JOYSTICK_HAT_MAX_NUMBER];
		glm::ivec2 _trackBalls[AGE_JOYSTICK_TRACKBALL_MAX_NUMBER];
	};
}
