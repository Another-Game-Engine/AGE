
#include "Engine.hh"
#include "Context/IRenderContext.hh"
#include "Timer.hh"
#include "Utils/PubSub.hpp"
#include "SceneManager.hh"
#include <Configuration.hpp>

#ifdef USE_IMGUI

#include <Utils/Age_Imgui.hpp>

#endif

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

#ifdef USE_IMGUI

	AGE::Imgui::getInstance()->startUpdate();

#endif

	timer->update();
	sceneManager->update(time);

	return (sceneManager->userUpdate(time));
}

void 		Engine::stop()
{
}
