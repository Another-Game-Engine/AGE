#include "Engine.hh"

#include <Core/PrepareRenderThread.hpp>
#include <Core/MainThread.hpp>
#include <Core/Timer.hh>

#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/RenderThread.hpp>

#include <Core/Commands/Render.hpp>

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

#include <Core/AssetsManager.hpp>
#include <Core/SceneManager.hh>
#include <Context/IRenderContext.hh>
#include <Core/Input.hh>
#include <Skinning/AnimationManager.hpp>

#endif

#ifdef USE_IMGUI

#include <Utils/Age_Imgui.hpp>

#endif

namespace AGE
{

	Engine::Engine()
		: _timer(nullptr)
	{

	}

	Engine::~Engine()
	{
	}

	void Engine::updateThreadStatistics(std::size_t id, float time)
	{
		auto &s = _threadsStatics[id];
		if (s.frameCounter >= s.frames.size())
		{
			s.frameCounter = 0;
		}
		s.frames[s.frameCounter++] = time;
		s.averageTime = 0.0f;
		for (auto i = 0; i < s.frames.size(); ++i)
		{
			s.averageTime += s.frames[i];
		}
		s.averageTime /= (float)(s.frames.size());
	}



//	bool Engine::_updateBegin()
//	{

//		return true;
//	}
//
//	bool Engine::_updateEnd()
//	{

//		return res;
//	}

	bool Engine::launch(std::function<bool()> &fn)
	{
		_timer = setInstance<Timer>();

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

		setInstance<AGE::AssetsManager>();
		setInstance<AGE::AnimationManager>();

		//_threadsStatics.insert(std::make_pair(std::this_thread::get_id().hash(), Engine::ThreadStatistics()));
		//_threadsStatics[std::this_thread::get_id().hash()].name = "Main Thread";

#endif //USE_DEFAULT_ENGINE_CONFIGURATION

		if (!fn())
			return false;
		return GetMainThread()->run();
	}

	bool Engine::update()
	{
		bool res = false;

		// TODO
		//updateThreadStatistics(_threadId, std::chrono::duration_cast<std::chrono::milliseconds>(_elapsed).count());
#ifdef USE_IMGUI
		AGE::Imgui::getInstance()->startUpdate();
#endif
		_timer->update();
		updateScenes(_timer->getElapsed());
		res = userUpdateScenes(_timer->getElapsed());
		if (!res)
			return false;
		if (ImGui::CollapsingHeader("Threads statistics"))
		{
			for (auto &e : _threadsStatics)
			{
				ImGui::Text("Thread : %s", e.second.name.c_str());
				ImGui::Text(std::string(std::to_string(e.second.averageTime) + " | " + std::to_string(int(1000.0f / e.second.averageTime)) + " fps").c_str());
				ImGui::PlotLines("Frame Times", e.second.frames.data() , (int)e.second.frames.size(), (int)e.second.frameCounter, e.second.name.c_str(), 0.0f, 20.0f, ImVec2(0, 70));
			}
		}
#ifdef USE_IMGUI
	ImGui::Render();
#endif
	GetPrepareThread()->getQueue()->emplaceCommand<Commands::Render::Flush>();
	}
}