
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

bool        Engine::init()
{
	auto context = getInstance<IRenderContext>();

	if (!context->start(1920, 1080, "Mini solar system"))
		return (false);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit Failed" << std::endl;
		return (false);
	}
	if (!getInstance<Renderer>()->init())
		return false;
  	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

bool 		Engine::start()
{
	//if (!getInstance<SceneManager>().startScene())
	//	return (false);
	return (true);
}

bool 		Engine::update()
{
	auto context = getInstance<IRenderContext>();

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto timer = getInstance<Timer>();
	auto inputs = getInstance<Input>();
	auto sceneManager = getInstance<SceneManager>();
	auto time = timer->getElapsed();

	timer->update();
    inputs->clearInputs();
	context->updateEvents(*inputs.get());
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
