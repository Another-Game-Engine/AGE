
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Utils/OpenGL.hh"
#include "Timer.hh"
#include "Renderer.hh"
#include "Utils/PubSub.hpp"
#include "SceneManager.hh"

#include <iostream>

Engine::Engine()
{

}

Engine::~Engine()
{
}

bool        Engine::init(int mode, unsigned int swidth, unsigned int sheight, std::string &&name)
{
	auto context = getInstance<IRenderContext>();

	if (!context->start(mode, swidth, sheight, std::move(name)))
		return (false);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit Failed" << std::endl;
		return (false);
	}
	if (!getInstance<Renderer>()->init())
		return false;
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

bool 		Engine::start()
{
	return (true);
}

bool 		Engine::update()
{
	auto context = getInstance<IRenderContext>();

	auto timer = getInstance<Timer>();
	auto inputs = getInstance<Input>();
	auto sceneManager = getInstance<SceneManager>();
	auto time = timer->getElapsed();

	timer->update();
    inputs->clearInputs();
	context->updateEvents(*inputs);
	sceneManager->update(time);
	context->flush();

	return (sceneManager->userUpdate(time));
}

void 		Engine::stop()
{
	auto renderer = getInstance<Renderer>();
	auto context = getInstance<IRenderContext>();

	renderer->uninit();
	context->stop();
}
