#include "Engine.hh"

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
		auto &stats = GetThreadManager()->getStatistics();
		for (auto i = (std::size_t)Thread::Main; i < Thread::hardwareConcurency(); ++i)
		{
			auto &e = stats[i];
			std::lock_guard<std::mutex>(e.mutex);
			for (auto i = 0; i < e.wait.size(); ++i)
			{
				e.waitCopy[i] = (float)e.wait[i] / 1000.0f;
				e.workCopy[i] = (float)e.work[i] / 1000.0f;
				e.totalCopy[i] = e.waitCopy[i] + e.workCopy[i];
			}
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
				if (ImGui::CollapsingHeader(e.name.c_str(), (const char*)0, true, true))
				{
					if (e.averageWaitTime + e.averageWorkTime != 0)
						ImGui::Text(std::string("Total : " + std::to_string(e.averageWaitTimeCopy + e.averageWorkTimeCopy) + " ms.").c_str());
					ImGui::Text(std::string("Work : " + std::to_string((float)e.averageWorkTimeCopy) + " ms.").c_str());
					ImGui::Text(std::string("Wait : " + std::to_string((float)e.averageWaitTimeCopy) + " ms.").c_str());
					if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
						ImGui::Text(std::string("FPS : " + std::to_string((int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy))) + ".").c_str());
					ImGui::PlotLines("Frame Times", e.totalCopy.data(), (int)e.totalCopy.size(), (int)e.frameCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(0, 70));
					if (ImGui::TreeNode((void*)(&e), "Details"))
					{
						ImGui::PlotLines("Wait Times", e.waitCopy.data(), (int)e.waitCopy.size(), (int)e.frameCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(0, 70));
						ImGui::PlotLines("Work Times", e.workCopy.data(), (int)e.workCopy.size(), (int)e.frameCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(0, 70));
						ImGui::TreePop();
					}
				}
			}
			if (ImGui::CollapsingHeader("Workers"))
			{
				ImGui::Columns((int)(Thread::hardwareConcurency() - 3), "Workers", true);
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					ImGui::Text(e.name.c_str());
					ImGui::NextColumn();
				}
				ImGui::Separator();
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					ImGui::Text("Total : %f", e.averageWaitTimeCopy + e.averageWorkTimeCopy);
					ImGui::NextColumn();
				}
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					// total
					ImGui::PlotLines("", e.totalCopy.data(), (int)e.totalCopy.size(), (int)e.frameCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(0, 70));
					ImGui::NextColumn();
				}
				ImGui::Separator();
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					ImGui::Text("Work : %f", e.averageWorkTimeCopy);
					ImGui::NextColumn();
				}
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					// work
					ImGui::PlotLines("", e.workCopy.data(), (int)e.workCopy.size(), (int)e.frameCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(0, 70));
					ImGui::NextColumn();
				}
				ImGui::Separator();
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					ImGui::Text("Wait : %f", e.averageWaitTimeCopy);
					ImGui::NextColumn();
				}
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					// wait
					ImGui::PlotLines("", e.waitCopy.data(), (int)e.waitCopy.size(), (int)e.frameCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(0, 70));
					ImGui::NextColumn();
				}
				ImGui::Separator();
			}
		}
		ImGui::End();
		if (!ImGui::Begin("Example: Fixed OverlayFPS OVERLAY", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
		}
		else
		{
			ImGui::SetWindowPos(ImVec2(10, 10));
			auto &e = GetThreadManager()->getStatistics()[Thread::Render];
			if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
				ImGui::Text("FPS : %i.", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			ImGui::End();
		}
#endif
#ifdef USE_IMGUI
		ImGui::Render();
#endif
		GetPrepareThread()->getQueue()->emplaceCommand<Commands::Render::Flush>();
		return true;
	}
}