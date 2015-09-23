#pragma once

#include <context/IRenderContext.hh>
#include <sdl/sdl_joystick.h>
#include <Core/Inputs/AgeInputs.hh>

class SDL_Window;
typedef int32_t SDL_JoystickID;
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

namespace AGE
{
	class Input;

	class SdlContext : public IRenderContext
	{
	public:
		SdlContext();
		virtual ~SdlContext();

		virtual void swapContext();
		virtual void refreshInputs();
		virtual const glm::uvec2 &getScreenSize();
		virtual void setScreenSize(const glm::uvec2 &screenSize);
		virtual void grabMouse(bool grabMouse);

	protected:
		virtual bool _init();

	private:
		SDL_Window		*_window;
		void	*_glContext;
		bool _firstCall;

		struct SdlJoystickInfos
		{
			SDL_JoystickID id;
			SDL_Joystick *handler;
		};

		SdlJoystickInfos _joysticks[AGE_JOYSTICK_MAX_NUMBER];

		uint32_t _fromSdlJoystickIdToAge(SDL_JoystickID id);
		void _initJoysticks(Input &inputs);
		void _addJoystick(Input &inputs, int joyIdx);
		void _removeJoystick(Input &inputs, SDL_JoystickID joyId);
	};
}