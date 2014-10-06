
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
#ifdef USE_IMGUI
	auto frameStart = std::chrono::high_resolution_clock::now();
#endif
	auto timer = getInstance<Timer>();
	auto sceneManager = getInstance<SceneManager>();
	auto time = timer->getElapsed();

#ifdef USE_IMGUI
IMGUI_BEGIN
	AGE::Imgui::getInstance()->startUpdate();
IMGUI_END
#endif

	timer->update();
	sceneManager->update(time);

	auto res =sceneManager->userUpdate(time);

#ifdef USE_IMGUI
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
	IMGUI_BEGIN
		ImGui::Text("Main Thread : %i ms", t.count());
	IMGUI_END
#endif
#ifdef USE_IMGUI
	AGE::Imgui::getInstance()->threadLoopEnd();
#endif

	return res;
}

void 		Engine::stop()
{
}
