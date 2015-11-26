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
		friend class Joystick;
	public:
		Input();
		Input(Input const &oth) = delete;
		virtual ~Input() = default;

		// Functions called by the user
		glm::ivec2  	    getMousePosition();
		glm::ivec2      	getMouseDelta();
		glm::ivec2      	getMouseWheel();
		bool 				getWindowInput(AgeWindowInputs input);
		bool 				getMouseButtonPressed(AgeMouseButtons input);
		bool 				getMouseButtonJustPressed(AgeMouseButtons input);
		bool 				getMouseButtonJustReleased(AgeMouseButtons input);
		bool 				getPhysicalKeyPressed(AgeKeys input);
		bool 				getMappedKeyPressed(AgeKeys input);
		bool 				getPhysicalKeyJustPressed(AgeKeys input);
		bool 				getMappedKeyJustPressed(AgeKeys input);
		bool 				getPhysicalKeyJustReleased(AgeKeys input);
		bool 				getMappedKeyJustReleased(AgeKeys input);
		bool				getJoystick(uint32_t joyId);
		bool				getJoystick(uint32_t joyId, Joystick &joystickInfos);

		// Function called by the context
		void				frameUpdate();
		void				resetInputs();

		void 				mouseButtonPressed(AgeMouseButtons input);
		void 				mouseButtonReleased(AgeMouseButtons input);
		void				addWindowInput(AgeWindowInputs input);
		void 				keyInputPressed(AgeKeys mappedInput, AgeKeys physicalInput);
		void 				keyInputReleased(AgeKeys mappedInput, AgeKeys physicalInput);
		void 				setMousePosition(glm::ivec2 const &pos, glm::ivec2 const &rel);
		void				setMouseWheel(glm::ivec2 const &delta);
		void				addJoystick(std::string const &name, uint32_t joyId);
		void				removeJoystick(uint32_t joyId);
		void				setJoystickAxis(uint32_t joyId, AgeJoystickAxis joyAxis, float value);
		void				setJoystickTrackBall(uint32_t joyId, uint32_t trackBallIdx, glm::ivec2 const &pos);
		void				setJoystickHat(uint32_t joyId, uint32_t joyHat, AgeJoystickHatDirections dir);
		void				joystickButtonPressed(uint32_t joyId, AgeJoystickButtons joyButton);
		void				joystickButtonReleased(uint32_t joyId, AgeJoystickButtons joyButton);

		// If IMGUI is activated, send the current state of the mouse to the render thread
		void				sendMouseStateToIMGUI();

	private:
		Joystick _joysticks[AGE_JOYSTICK_MAX_NUMBER];
		bool _windowInputs[AgeWindowInputs::AGE_WINDOW_INPUT_NUMBER];
		uint8_t _mouseInputs[AgeMouseButtons::AGE_MOUSE_BUTTONS_NUMBER];
		uint8_t _keyInputs[AgeKeys::AGE_KEY_NUMBER];

		std::uint32_t _mousePosX;
		std::uint32_t _mousePosY;
		std::uint32_t _mouseDelX;
		std::uint32_t _mouseDelY;
		std::uint32_t _mouseWheelX;
		std::uint32_t _mouseWheelY;

		AGE::SpinLock      _mutex;
	};

}