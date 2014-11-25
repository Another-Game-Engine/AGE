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
		_renderThread = setInstance<AGE::RenderThread, AGE::Threads::Render>();
		_prepareThread = setInstance<AGE::Threads::Prepare>();

		_timer = setInstance<Timer>();

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

		_sceneManager = setInstance<SceneManager>();
		setInstance<AGE::AssetsManager>();
		setInstance<Input>();
		setInstance<AGE::AnimationManager>();

		_prepareThread->setNextCommandQueue(_renderThread);
		_renderThread->setNextCommandQueue(this);
		setNextCommandQueue(_prepareThread);

		getCurrentThreadCommandQueue()->setWaitingTime(1);
		_renderThread->getCurrentThreadCommandQueue()->setWaitingTime(1);
		_prepareThread->getCurrentThreadCommandQueue()->setWaitingTime(1);

		_renderThread->setLastOfLoop(true);

#ifdef USE_IMGUI
		registerMessageCallback<MTC::FrameTime>([&](MTC::FrameTime& msg)
		{
			ImGui::Text((std::string(msg.name) + " " + std::to_string(msg.time)).c_str());
		});
#endif

		_renderThread->launch(this, "Render thread");
		_prepareThread->launch(this, "Prepare thread");

#endif //USE_DEFAULT_ENGINE_CONFIGURATION

		return true;
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
		//getCurrentThreadCommandQueue()->releaseReadability();
		//commandQueueUpdate();
		_sceneManager->update(time);
		res = _sceneManager->userUpdate(time);
		//getCommandQueue()->releaseReadability();
#endif
		return res;
	}
}