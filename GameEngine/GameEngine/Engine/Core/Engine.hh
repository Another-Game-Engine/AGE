#pragma once

#include <Configuration.hpp>
#include "Utils/DependenciesInjector.hpp"

namespace AGE
{
	class PrepareRenderThread;
	class RenderThread;
	class MainThread;
	class SceneManager;
	class Timer;

	class Engine : public DependenciesInjector
	{
	protected:
		Engine(Engine const &);
		Engine &operator=(Engine const &);

		PrepareRenderThread *_prepareThread;
		RenderThread *_renderThread;
		MainThread *_mainThread;
		Timer *_timer;
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		SceneManager *_sceneManager;
#endif

	public:
		// All functions are virtual so if you want to customize your Engine class
		// without hacking the core, you can heritate from :)
		Engine();
		virtual ~Engine();

		virtual bool        init();
		virtual bool 		start();
		virtual bool 		update();
		virtual void 		stop();

		inline PrepareRenderThread *getPrepareThread()
		{
			return _prepareThread;
		}
		inline RenderThread *getRenderThread()
		{
			return _renderThread;
		}
		inline MainThread *getMainThread()
		{
			return _mainThread;
		}
	};
}