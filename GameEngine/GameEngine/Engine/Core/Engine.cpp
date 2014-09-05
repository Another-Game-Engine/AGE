
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Timer.hh"
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
//	auto inputs = getInstance<Input>();
	auto sceneManager = getInstance<SceneManager>();
	auto time = timer->getElapsed();

	timer->update();
//    inputs->clearInputs();
//	context->updateEvents(*inputs);
	sceneManager->update(time);
	context->flush();

	return (sceneManager->userUpdate(time));
}

void 		Engine::stop()
{
	auto context = hasInstance<IRenderContext>() ? getInstance<IRenderContext>() : nullptr;

	if (context)
		context->stop();
}
