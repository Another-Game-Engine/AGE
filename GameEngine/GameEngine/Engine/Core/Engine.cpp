#include "Engine.hh"

#include <Core/Timer.hh>

#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/RenderThread.hpp>

#include <Core/Commands/ToRender.hpp>

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

#include <AssetManagement/AssetManager.hh>
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

#ifdef USE_IMGUI
		AGE::Imgui::getInstance()->startUpdate();
#endif
		_timer->update();
		updateScenes(_timer->getElapsed());
		res = userUpdateScenes(_timer->getElapsed());
		if (!res)
			return false;
		static double refreshStats = 0.0;
		refreshStats += _timer->getElapsed();
		auto &stats = GetThreadManager()->getStatistics();
		if (refreshStats >= 1.0)
		{
			for (auto i = (std::size_t)Thread::Main; i < Thread::hardwareConcurency(); ++i)
			{
				auto &e = stats[i];
				e.waitCopy[e.secondCounter] = e.averageWaitTime;
				e.workCopy[e.secondCounter] = e.averageWorkTime;
				e.totalCopy[e.secondCounter] = e.averageWaitTime + e.averageWorkTime;
				++e.secondCounter;
				if (e.secondCounter >= e.waitCopy.size())
					e.secondCounter = 0;
			}
			refreshStats = 0.0;
		}
		for (auto i = (std::size_t)Thread::Main; i < Thread::hardwareConcurency() ; ++i)
		{
			auto &e = stats[i];
			e.averageWaitTimeCopy = e.averageWaitTime;
			e.averageWorkTimeCopy = e.averageWorkTime;
		}
		#ifdef USE_IMGUI
		if (ImGui::Begin("Threads statistics", (bool*)0, ImVec2(0, 0), -1.0f, ImGuiWindowFlags_AlwaysAutoResize))
		{
			auto &stats = GetThreadManager()->getStatistics();
			for (auto i = (std::size_t)Thread::Main; i <= Thread::Render; ++i)
			{
				auto &e = stats[i];
				e.averageWaitTimeCopy = e.averageWaitTime;
				e.averageWorkTimeCopy = e.averageWorkTime;
				if (ImGui::CollapsingHeader(e.name.c_str(), (const char*)0, true, true))
				{
					if (e.averageWaitTime + e.averageWorkTime != 0)
						ImGui::Text(std::string("Total : " + std::to_string(e.averageWaitTimeCopy + e.averageWorkTimeCopy) + " ms.").c_str());
					ImGui::Text(std::string("Work : " + std::to_string((float)e.averageWorkTimeCopy) + " ms.").c_str());
					ImGui::Text(std::string("Wait : " + std::to_string((float)e.averageWaitTimeCopy) + " ms.").c_str());
					if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
						ImGui::Text(std::string("FPS : " + std::to_string((int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy))) + ".").c_str());
					ImGui::PlotLines("Frame Times", e.totalCopy.data(), (int)e.totalCopy.size(), (int)e.secondCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(200, 30));
					if (ImGui::TreeNode((void*)(&e), "Details"))
					{
						ImGui::PlotLines("Wait Times", e.waitCopy.data(), (int)e.waitCopy.size(), (int)e.secondCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(200, 30));
						ImGui::TreePop();
					}
				}
			}
			if (ImGui::CollapsingHeader("Workers"))
			{
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					ImGui::Text("%s", e.name.c_str());
					ImGui::PlotLines("", e.workCopy.data(), (int)e.workCopy.size(), (int)e.secondCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(200, 30));
					ImGui::Separator();
				}
			}
		}
		ImGui::End();
		if (!ImGui::Begin("Example: Fixed OverlayFPS OVERLAY", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
		}
		else
		{
			ImGui::SetWindowPos(ImVec2(10, 10));
			{
				auto &e = GetThreadManager()->getStatistics()[Thread::Main];
				if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
					ImGui::Text("Main : %i fps", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			}
			{
				auto &e = GetThreadManager()->getStatistics()[Thread::PrepareRender];
				if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
					ImGui::Text("Prepare : %i fps", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			}
			{
				auto &e = GetThreadManager()->getStatistics()[Thread::Render];
				if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
					ImGui::Text("Render : %i fps", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			}
			ImGui::End();
		}
#endif
#ifdef USE_IMGUI
		ImGui::Render();
#endif
		GetPrepareThread()->getQueue()->emplaceCommand<Commands::ToRender::Flush>();
		return true;
	}
}