#include <Context/IRenderContext.hh>
#include <Core/Inputs/Input.hh>
#include <tmq/message.hpp>
#include <Configuration.hpp>

#ifdef USE_IMGUI
#include <Utils/Age_Imgui.hpp>
#endif

namespace AGE
{
	IRenderContext::IRenderContext()
		: _screenSize(glm::uvec2(10, 10))
		, _windowName("<3 ~AGE~ <3")
	{ }

	IRenderContext::~IRenderContext()
	{
	}

	bool IRenderContext::init(unsigned int swidth, unsigned int sheight, std::string && name)
	{
		_windowName = name;
		_screenSize = glm::uvec2(swidth, sheight);
		return _init();
	}

	glm::uvec2 IRenderContext::getScreenSize() const
	{
		return _screenSize;
	}

	AgeKeys IRenderContext::findAgeKey(int contextKey) const
	{
		auto it = _toAgeKey.find(contextKey);

		if (it == _toAgeKey.end())
			return (AGE_KEY_UNKNOWN);
		return (it->second);
	}

	AgeInputs IRenderContext::findAgeInput(int contextInput) const
	{
		auto it = _toAgeInput.find(contextInput);

		if (it == _toAgeInput.end())
			return (AGE_INPUT_UNKNOWN);
		return (it->second);
	}

	AgeJoystickAxis IRenderContext::findAgeJoystickAxis(int contextInput) const
	{
		auto it = _toAgeJoystickAxis.find(contextInput);

		if (it == _toAgeJoystickAxis.end())
			return (AGE_JOYSTICK_AXIS_UNKNOWN);
		return (it->second);
	}

	AgeJoystickButtons IRenderContext::findAgeJoystickButton(int contextInput) const
	{
		auto it = _toAgeJoystickButtons.find(contextInput);

		if (it == _toAgeJoystickButtons.end())
			return (AGE_JOYSTICK_BUTTON_UNKNOWN);
		return (it->second);
	}

	AgeJoystickHatDirections IRenderContext::findAgeJoystickHatDirection(int contextInput) const
	{
		auto it = _toAgeJoystickHatDirections.find(contextInput);

		if (it == _toAgeJoystickHatDirections.end())
			return (AGE_JOYSTICK_HAT_CENTERED);
		return (it->second);
	}
}