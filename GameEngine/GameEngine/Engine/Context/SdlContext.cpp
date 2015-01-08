#include "context/SdlContext.hh"
#include "Utils/OpenGL.hh"
#include <iostream>
#include <Utils/DependenciesInjector.hpp>
#include <Core/Input.hh>
#include <Render/RenderManager.hh>

bool SdlContext::_init(int mode)
{
	_dependencyManager.lock()->setInstance<Input>();

	if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0 ||
		//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) != 0 ||
		(_window = SDL_CreateWindow(_windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		_screenSize.x, _screenSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | mode )) == NULL ||
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
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return (true);
}

