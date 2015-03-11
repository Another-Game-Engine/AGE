#pragma once

#include <list>
#include <Utils/Dependency.hpp>
#include <glm/gtc/type_precision.hpp>
#include <Core/Inputs/AgeInputs.hh>
#include <mutex>
#include <atomic>
#include <Utils/SpinLock.hpp>
#include <Core/Inputs/Joystick.hh>
#include <array>

namespace AGE
{
	class Input : public Dependency < Input >
	{
	private:
		Joystick _joysticks[AGE_JOYSTICK_MAX_NUMBER];
		std::array<bool, AGE_INPUT_NUMBER> _inputs;
		std::array<bool, AGE_KEY_NUMBER> _keyPhysicalInputs;
		std::array<bool, AGE_KEY_NUMBER> _keyMappedInputs;

		std::atomic_int32_t _mousePosX;
		std::atomic_int32_t _mousePosY;
		std::atomic_int32_t _mouseDelX;
		std::atomic_int32_t _mouseDelY;
		std::atomic_int32_t _mouseWheelX;
		std::atomic_int32_t _mouseWheelY;

		AGE::SpinLock      _mutex;
	public:
		Input();
		virtual ~Input() { }

		// Function called by the context
		void 				addInput(AgeInputs input);
		void 				removeInput(AgeInputs input);
		void 				keyInputPressed(AgeKeys mappedInput, AgeKeys physicalInput);
		void 				keyInputReleased(AgeKeys mappedInput, AgeKeys physicalInput);
		void 				setMousePosition(glm::ivec2 const &pos, glm::ivec2 const &rel);
		void				setMouseWheel(glm::ivec2 const &delta);
		void				addJoystick(std::string const &name, uint32_t joyId);
		void				removeJoystick(uint32_t joyId);
		void				setJoystickAxis(uint32_t joyId, AgeJoystickAxis joyAxis, float value);
		void				setJoystickTrackBall(uint32_t joyId, glm::ivec2 const &pos);
		void				setJoystickHat(uint32_t joyId, uint32_t joyHat, AgeJoystickHatDirections dir);
		void				joystickButtonPressed(uint32_t joyId, AgeJoystickButtons joyButton);
		void				joystickButtonReleased(uint32_t joyId, AgeJoystickButtons joyButton);

		// If IMGUI is activated, send the current state of the mouse to the render thread
		void				sendMouseStateToIMGUI();

		// Functions called by the user
		glm::ivec2  	    getMousePosition();
		glm::ivec2      	getMouseDelta();
		glm::ivec2      	getMouseWheel();
		bool 				getInput(AgeInputs input, bool handled = false);
		bool 				getPhysicalKey(AgeKeys input, bool handled = false);
		bool 				getMappedKey(AgeKeys input, bool handled = false);
		bool				getJoystick(uint32_t joyId);
		bool				getJoystick(uint32_t joyId, Joystick &joystickInfos);
	};

}