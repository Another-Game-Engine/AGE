#pragma once

#include <glm/glm.hpp>
#include <Utils/Dependency.hpp>
#include <Core/Inputs/AgeInputs.hh>
#include <string>
#include <map>

namespace AGE
{
	class IRenderContext : public Dependency < IRenderContext >
	{
	public:
		IRenderContext();
		virtual ~IRenderContext();

		bool init(unsigned int swidth, unsigned int sheight, std::string && name);
		glm::uvec2 getScreenSize() const;

		virtual void swapContext() = 0;
		virtual void refreshInputs() = 0;
		virtual void setScreenSize(const glm::uvec2 &screenSize) = 0;

	protected:
		glm::uvec2 _screenSize;
		std::string _windowName;

		static std::map<int, AgeKeys> _toAgeKey;
		static std::map<int, AgeInputs> _toAgeInput;
		static std::map<int, AgeJoystickAxis> _toAgeJoystickAxis;
		static std::map<int, AgeJoystickButtons> _toAgeJoystickButtons;
		static std::map<int, AgeJoystickHatDirections> _toAgeJoystickHatDirections;

		AgeKeys findAgeKey(int contextKey) const;
		AgeInputs findAgeInput(int contextInput) const;
		AgeJoystickAxis findAgeJoystickAxis(int contextInput) const;
		AgeJoystickButtons findAgeJoystickButton(int contextInput) const;
		AgeJoystickHatDirections findAgeJoystickHatDirection(int contextInput) const;

		virtual bool _init() = 0;
	};
}