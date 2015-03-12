#pragma once

#include <Core/Inputs/AgeInputs.hh>

namespace AGE
{
	class ActionManager
	{
	public:
//		struct KeyboardAction
//		{
//			AgeKeys key;
//			bool physical;
//			bool wasPressed;
//			bool pressed;
//			bool wasReleased;
//		};
//
//		struct JoystickAction
//		{
//			uint32_t joystickIdx;
//		};
//
//		template<typename T>
//		struct TypeAction
//		{
//			typedef std::function<T(T currentValue)> handlerFunction;
//
//			T value;
//			std::map<AgeKeys, handlerFunction> physicalKeyHandlers;
//			std::map<AgeKeys, handlerFunction> mappedKeyHandlers;
//			std::map<JoystickAction, handlerFunction> joystickActionHandlers;
//		};

	public:
		ActionManager();
		~ActionManager();

//		void registerFloatAction(std::string const &name, AgeKeys key, std::function<float(float currentValue)> handler);
//		void registerBoolAction(std::string const &name, AgeKeys key, std::function<bool(bool currentValue)> handler);

	private:
//		std::map<TypeAction<float>> _floatActions;
//		std::map<TypeAction<bool>> _boolActions;
	};
}
