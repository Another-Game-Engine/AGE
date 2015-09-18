#pragma once

#include <glm/glm.hpp>
#include <Utils/Dependency.hpp>
#include <string>
#include <map>

namespace AGE
{
	enum class AgeKeys;
	enum class AgeWindowInputs;
	enum class AgeMouseButtons;
	enum class AgeJoystickAxis;
	enum class AgeJoystickButtons;
	enum class AgeJoystickHatDirections;

	class IRenderContext : public Dependency < IRenderContext >
	{
	public:
		IRenderContext();
		virtual ~IRenderContext();

		bool init(unsigned int swidth, unsigned int sheight, std::string && name);
		inline glm::uvec2 getScreenSize() const { return _screenSize; }

		virtual void swapContext() = 0;
		virtual void refreshInputs() = 0;
		virtual void setScreenSize(const glm::uvec2 &screenSize) = 0;
		virtual void grabMouse(bool grabMouse) = 0;

	protected:
		glm::uvec2 _screenSize;
		std::string _windowName;

		static std::map<int, AgeKeys> _toAgeMappedKey;
		static std::map<int, AgeKeys> _toAgePhysicalKey;
		static std::map<int, AgeWindowInputs> _toAgeWindowInput;
		static std::map<int, AgeMouseButtons> _toAgeMouseButton;
		static std::map<int, AgeJoystickAxis> _toAgeJoystickAxis;
		static std::map<int, AgeJoystickButtons> _toAgeJoystickButtons;
		static std::map<int, AgeJoystickHatDirections> _toAgeJoystickHatDirections;

		AgeKeys findAgeMappedKey(int contextKey) const;
		AgeKeys findAgePhysicalKey(int contextKey) const;
		AgeWindowInputs findAgeWindowInput(int contextInput) const;
		AgeMouseButtons findAgeMouseButton(int contextInput) const;
		AgeJoystickAxis findAgeJoystickAxis(int contextInput) const;
		AgeJoystickButtons findAgeJoystickButton(int contextInput) const;
		AgeJoystickHatDirections findAgeJoystickHatDirection(int contextInput) const;

		virtual bool _init() = 0;
	};
}