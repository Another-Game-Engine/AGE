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
		, _mainThread(nullptr)
		, _timer(nullptr)
	{

	}

	Engine::~Engine()
	{
	}

	bool        Engine::init()
	{
		_renderThread = setInstance<AGE::RenderThread, AGE::Threads::Render>();
		_prepareThread = setInstance<AGE::Threads::Prepare>();
		_mainThread = setInstance<AGE::MainThread>();

		_timer = setInstance<Timer>();

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

		_sceneManager = setInstance<SceneManager>();
		setInstance<AGE::AssetsManager>();
		setInstance<Input>();
		setInstance<AGE::AnimationManager>();

		_prepareThread->setNextCommandQueue(_renderThread->getCurrentThreadCommandQueue());
		_renderThread->setNextCommandQueue(_mainThread->getCurrentThreadCommandQueue());
		_mainThread->setNextCommandQueue(_prepareThread->getCurrentThreadCommandQueue());

		_mainThread->getCurrentThreadCommandQueue()->setWaitingTime(100);
		_renderThread->getCurrentThreadCommandQueue()->setWaitingTime(100);
		_prepareThread->getCurrentThreadCommandQueue()->setWaitingTime(100);

		_renderThread->launch(this);
		_prepareThread->launch(this);
		_mainThread->init(this);

#endif //USE_DEFAULT_ENGINE_CONFIGURATION

		return true;
	}

	bool 		Engine::start()
	{
		return (true);
	}

	bool 		Engine::update()
	{

		auto time = _timer->getElapsed();
		_timer->update();
		bool res = true;
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
#ifdef USE_IMGUI
		AGE::Imgui::getInstance()->startUpdate();
#endif
		_mainThread->commandQueueUpdate();
		_sceneManager->update(time);
		 res = _sceneManager->userUpdate(time);
		 _mainThread->getCommandQueue()->releaseReadability();
#endif
		return res;
	}

	void 		Engine::stop()
	{
		_prepareThread->quit();
		_renderThread->quit();
	}
}
