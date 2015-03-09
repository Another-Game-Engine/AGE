#include <context/SDL/SdlContext.hh>
#include <Utils/OpenGL.hh>
#include <iostream>
#include <Utils/DependenciesInjector.hpp>
#include <Core/Input.hh>
#include <SDL/SDL.h>
#include <Core/Input.hh>
#include <Threads/MainThread.hpp>

namespace AGE
{
	bool SdlContext::_init(int mode)
	{
		_dependencyManager->setInstance<Input>();
		if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
			//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0 ||
			//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) != 0 ||
			(_window = SDL_CreateWindow(_windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			_screenSize.x, _screenSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | mode)) == NULL ||
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
//		glEnable(GL_ALPHA_TEST);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		while (SDL_PollEvent(&events))
		{
			switch (events.type)
			{
			case SDL_KEYDOWN:
				input->addKeyInput(findAgeKey(events.key.keysym.sym));
				break;
			case SDL_KEYUP:
				input->removeKeyInput(findAgeKey(events.key.keysym.sym));
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
}
