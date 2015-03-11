#include <context/SDL/SdlContext.hh>
#include <Utils/OpenGL.hh>
#include <iostream>
#include <Utils/DependenciesInjector.hpp>
#include <Core/Inputs/Input.hh>
#include <SDL/SDL.h>
#include <Core/Inputs/Input.hh>
#include <Threads/MainThread.hpp>

namespace AGE
{
	bool SdlContext::_init()
	{
		_firstCall = true;
		_dependencyManager->setInstance<Input>();
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0 ||
			//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0 ||
			//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) != 0 ||
			(_window = SDL_CreateWindow(_windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			_screenSize.x, _screenSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS)) == NULL ||
			(_glContext = SDL_GL_CreateContext(_window)) == NULL)
		{
			std::cerr << "SDL_GL_CreateContext Failed : " << SDL_GetError() << std::endl;
			return (false);
		}
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "glewInit Failed" << std::endl;
			return (false);
		}
		return (true);
	}

	SdlContext::SdlContext() { }

	SdlContext::~SdlContext() { }

	void SdlContext::swapContext()
	{
		SDL_GL_SwapWindow(_window);
	}

	void SdlContext::refreshInputs()
	{
		SDL_Event events;
		auto input = _dependencyManager->getInstance<Input>();

		input->frameUpdate();
		if (_firstCall)
		{
			_initJoysticks(*input);
			_firstCall = false;
		}
		while (SDL_PollEvent(&events))
		{
			switch (events.type)
			{
			case SDL_KEYDOWN:
				input->keyInputPressed(findAgeMappedKey(events.key.keysym.sym), findAgePhysicalKey(events.key.keysym.scancode));
				break;
			case SDL_KEYUP:
				input->keyInputReleased(findAgeMappedKey(events.key.keysym.sym), findAgePhysicalKey(events.key.keysym.scancode));
				break;
			case SDL_MOUSEBUTTONDOWN:
				input->addInput(findAgeInput(events.button.button));
				break;
			case SDL_MOUSEBUTTONUP:
				input->removeInput(findAgeInput(events.button.button));
				break;
			case SDL_MOUSEWHEEL:
				input->setMouseWheel(glm::ivec2(events.wheel.x, events.wheel.y));
				break;
			case SDL_MOUSEMOTION:
				input->setMousePosition(glm::ivec2(events.motion.x, events.motion.y), glm::ivec2(events.motion.xrel, events.motion.yrel));
				break;
			case SDL_JOYDEVICEADDED:
				_addJoystick(*input, events.jdevice.which);
				break;
			case SDL_JOYDEVICEREMOVED:
				_removeJoystick(*input, events.jdevice.which);
				break;
			case SDL_JOYAXISMOTION:
			{
				uint32_t joyId = _fromSdlJoystickIdToAge(events.jaxis.which);
				AgeJoystickAxis joyAxis = findAgeJoystickAxis(events.jaxis.axis);
				float value = (float)events.jaxis.value / 32768.0f;
				input->setJoystickAxis(joyId, joyAxis, value);
			}
				break;
			case SDL_JOYBUTTONDOWN:
			{
				uint32_t joyId = _fromSdlJoystickIdToAge(events.jbutton.which);
				AgeJoystickButtons joyButton = findAgeJoystickButton(events.jbutton.button);
				input->joystickButtonPressed(joyId, joyButton);
			}
			break;
			case SDL_JOYBUTTONUP:
			{
				uint32_t joyId = _fromSdlJoystickIdToAge(events.jbutton.which);
				AgeJoystickButtons joyButton = findAgeJoystickButton(events.jbutton.button);
				input->joystickButtonReleased(joyId, joyButton);
			}
			break;
			case SDL_JOYBALLMOTION:
			{
				uint32_t joyId = _fromSdlJoystickIdToAge(events.jball.which);
				input->setJoystickTrackBall(joyId, glm::ivec2(events.jball.xrel, events.jball.yrel));
			}
			break;
			case SDL_JOYHATMOTION:
			{
				uint32_t joyId = _fromSdlJoystickIdToAge(events.jhat.which);
				AgeJoystickHatDirections joyHatDir = findAgeJoystickHatDirection(events.jhat.value);
				assert(events.jhat.hat < AGE_JOYSTICK_HAT_MAX_NUMBER);
				input->setJoystickHat(joyId, events.jhat.hat, joyHatDir);
			}
			break;
			case SDL_WINDOWEVENT:
				// handle windows events
				break;
			default:
				input->addInput(findAgeInput(events.type));
				break;
			}
		}
		input->sendMouseStateToIMGUI();
	}

	const glm::uvec2 &SdlContext::getScreenSize()
	{
		return _screenSize;
	}

	void SdlContext::setScreenSize(const glm::uvec2 &screenSize)
	{
		_screenSize = screenSize;
		SDL_SetWindowSize(_window, _screenSize.x, _screenSize.y);
		SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	uint32_t SdlContext::_fromSdlJoystickIdToAge(SDL_JoystickID id)
	{
		uint32_t currentId = 0;

		while (currentId != AGE_JOYSTICK_MAX_NUMBER &&
			_joysticks[currentId].id != id)
			++currentId;
		assert(currentId != AGE_JOYSTICK_MAX_NUMBER && "Cannot find the joystick.");
		return (currentId);
	}

	void SdlContext::_initJoysticks(Input &inputs)
	{
		// Init Joysticks
		for (int i = 0; i < AGE_JOYSTICK_MAX_NUMBER; ++i)
		{
			_joysticks[i].id = -1;
			_joysticks[i].handler = NULL;
		}
		SDL_JoystickEventState(SDL_ENABLE);
		for (int i = 0; i < SDL_NumJoysticks(); ++i)
			_addJoystick(inputs, i);
	}

	void SdlContext::_addJoystick(Input &inputs, int joyIdx)
	{
		SDL_Joystick *current = SDL_JoystickOpen(joyIdx);

		if (current)
		{
			SDL_JoystickID sdlJoyId = SDL_JoystickInstanceID(current);

			if (sdlJoyId != -1)
			{
				uint32_t currentId = 0;

				while (currentId != AGE_JOYSTICK_MAX_NUMBER &&
					_joysticks[currentId].id != -1)
					++currentId;
				assert(currentId != AGE_JOYSTICK_MAX_NUMBER && "AGE cannot handle more than 6 joysticks.");
				_joysticks[currentId].id = sdlJoyId;
				_joysticks[currentId].handler = current;
				inputs.addJoystick(SDL_JoystickName(current), currentId);
			}
		}
	}

	void SdlContext::_removeJoystick(Input &inputs, SDL_JoystickID joyId)
	{
		uint32_t currentId = 0;

		while (currentId != AGE_JOYSTICK_MAX_NUMBER &&
			_joysticks[currentId].id != joyId)
			++currentId;
		assert(currentId != AGE_JOYSTICK_MAX_NUMBER && "Cannot remove the nonexistent joystick");
		SDL_JoystickClose(_joysticks[currentId].handler);
		_joysticks[currentId].id = -1;
		_joysticks[currentId].handler = NULL;
		inputs.removeJoystick(currentId);
	}

}
