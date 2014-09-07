
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Timer.hh"
#include "Utils/PubSub.hpp"
#include "SceneManager.hh"
#include <Core/DefaultQueues/RenderThread.hpp>

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
	auto timer = getInstance<Timer>();
	auto sceneManager = getInstance<SceneManager>();
	auto time = timer->getElapsed();
	auto renderThread = getInstance < AGE::DefaultQueue::RenderThread >();

	timer->update();
	renderThread->getCommandQueue().emplace<RendCtxCommand::RefreshInputs>();
	sceneManager->update(time);
	renderThread->getCommandQueue().emplace<RendCtxCommand::Flush>();
	renderThread->getCommandQueue().releaseReadability();

	return (sceneManager->userUpdate(time));
}

void 		Engine::stop()
{
}
