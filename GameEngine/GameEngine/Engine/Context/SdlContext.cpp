#include "context/SdlContext.hh"
#include "Utils/OpenGL.hh"
#include <iostream>
#include <Utils/DependenciesInjector.hpp>

bool SdlContext::_init(int mode)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0 ||
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) != 0 ||
		(_window = SDL_CreateWindow(_windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		_screenSize.x, _screenSize.y, SDL_WINDOW_OPENGL | mode)) == NULL ||
		(_glContext = SDL_GL_CreateContext(_window)) == NULL)
	{
		std::cerr << "SDL_GL_CreateContext Failed" << std::endl;
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

bool SdlContext::_update()
{
	bool returnValue = true;

	_commandQueue.getDispatcher()
		.handle<RendCtxCommand::Flush>([&](const RendCtxCommand::Flush& msg)
	{
		SDL_GL_SwapWindow(_window);
	})
		.handle<RendCtxCommand::GetScreenSize>([&](RendCtxCommand::GetScreenSize& msg)
	{
		msg.result.set_value(_screenSize);
	})
		.handle<RendCtxCommand::SetScreenSize>([&](const RendCtxCommand::SetScreenSize& msg)
	{
		_screenSize = msg.screenSize;
		SDL_SetWindowSize(_window, _screenSize.x, _screenSize.y);
		SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	})
		.handle<RendCtxCommand::BoolFunction>([&](RendCtxCommand::BoolFunction& msg)
	{
		msg.result.set_value(msg.function());
	})
		.handle<RendCtxCommand::Stop>([&](const RendCtxCommand::Stop& msg)
	{
		returnValue = false;
	});

	//SDL_Event events;
	//auto input = _dependencyManager.lock()->getInstance<Input>();
	//while (SDL_PollEvent(&events))
	//{
	//	if (events.type == SDL_KEYDOWN)
	//		input->addKeyInput(events.key.keysym.sym);
	//	else if (events.type == SDL_KEYUP)
	//		input->removeKeyInput(events.key.keysym.sym);
	//	else if (events.type == SDL_MOUSEBUTTONDOWN)
	//		input->addKeyInput(events.button.button);
	//	else if (events.type == SDL_MOUSEBUTTONUP)
	//		input->removeKeyInput(events.button.button);
	//	else if (events.type == SDL_MOUSEWHEEL)
	//		input->setMouseWheel(glm::i8vec2(events.wheel.x, events.wheel.y));
	//	else if (events.type == SDL_MOUSEMOTION)
	//		input->setMousePosition(glm::i8vec2(events.motion.x, events.motion.y), glm::i8vec2(events.motion.xrel, events.motion.yrel));
	//	else
	//		input->addInput(events.type);
	//}
	return returnValue;
}

