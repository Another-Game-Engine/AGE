#include "Engine.hh"

#include <Core/PrepareRenderThread.hpp>
#include <Core/RenderThread.hpp>
#include <Core/MainThread.hpp>

#include <Core/Timer.hh>

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
		: _prepareThread(nullptr)
		, _renderThread(nullptr)
		, _timer(nullptr)
	{

	}

	Engine::~Engine()
	{
	}

	bool Engine::_init()
	{
		_renderThread = setInstance<AGE::OldRenderThread, AGE::Threads::Render>();
		_prepareThread = setInstance<AGE::Threads::Prepare>();

		_timer = setInstance<Timer>();

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

		setInstance<AGE::AssetsManager>();
		setInstance<Input>();
		setInstance<AGE::AnimationManager>();

		_prepareThread->setNextCommandQueue(_renderThread);
		_renderThread->setNextCommandQueue(this);
		setNextCommandQueue(_prepareThread);

		getCurrentThreadCommandQueue()->setWaitingTime(1);
		_renderThread->getCurrentThreadCommandQueue()->setWaitingTime(1);
		_prepareThread->getCurrentThreadCommandQueue()->setWaitingTime(1);

		_threadsStatics.insert(std::make_pair(std::this_thread::get_id().hash(), Engine::ThreadStatistics()));
		_threadsStatics[std::this_thread::get_id().hash()].name = "Main Thread";

#ifdef USE_IMGUI
		registerMessageCallback<MTC::FrameTime>([this](MTC::FrameTime& msg)
		{
			updateThreadStatistics(msg.id, msg.time);
		});

		registerMessageCallback<MTC::ThreadRegistration>([this](MTC::ThreadRegistration& msg)
		{
			_threadsStatics.insert(std::make_pair(msg.id, Engine::ThreadStatistics()));
			_threadsStatics[msg.id].name = msg.name;
		});
#endif

		_renderThread->launch(this, "Render thread");
		_prepareThread->launch(this, "Prepare thread");

#endif //USE_DEFAULT_ENGINE_CONFIGURATION

		return true;
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

	bool Engine::_initInNewThread()
	{
		return true;
	}

	bool Engine::_release()
	{
		_prepareThread->quit();
		_renderThread->quit();
		return true;
	}

	bool Engine::_releaseInNewThread()
	{
		return true;
	}

	bool Engine::_updateBegin()
	{
		updateThreadStatistics(_threadId, std::chrono::duration_cast<std::chrono::milliseconds>(_elapsed).count());
#ifdef USE_IMGUI
		AGE::Imgui::getInstance()->startUpdate();
#endif
		return true;
	}

	bool Engine::_updateEnd()
	{
		auto time = _timer->getElapsed();
		_timer->update();
		bool res = true;
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		updateScenes(time);
		if (ImGui::CollapsingHeader("Threads statistics"))
		{
			for (auto &e : _threadsStatics)
			{
				ImGui::Text("Thread : %s", e.second.name.c_str());
				ImGui::Text(std::string(std::to_string(e.second.averageTime) + " | " + std::to_string(int(1000.0f / e.second.averageTime)) + " fps").c_str());
				ImGui::PlotLines("Frame Times", e.second.frames.data() , (int)e.second.frames.size(), (int)e.second.frameCounter, e.second.name.c_str(), 0.0f, 20.0f, ImVec2(0, 70));
			}
		}
		res = userUpdateScenes(time);
#endif
		return res;
	}
}