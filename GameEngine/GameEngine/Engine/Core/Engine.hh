#pragma once

#include <Configuration.hpp>
#include "Utils/DependenciesInjector.hpp"
#include <Utils/ThreadQueue.hpp>
#include <array>
#include <map>

namespace AGE
{
	class PrepareRenderThread;
	class RenderThread;
	class SceneManager;
	class Timer;

	class Engine : public DependenciesInjector, public CommandQueue
	{
	protected:
		Engine(Engine const &);
		Engine &operator=(Engine const &);

		PrepareRenderThread *_prepareThread;
		RenderThread *_renderThread;
		Timer *_timer;

		struct ThreadStatistics
		{
			std::string name;
			float averageTime;
			unsigned char frameCounter;
			std::array < float, 32 > frames;
			ThreadStatistics()
				: name("")
				, averageTime(0.0f)
				, frameCounter(0)
			{
				frames.fill(0);
			}
		};

		std::map<std::size_t, ThreadStatistics> _threadsStatics;
		void updateThreadStatistics(std::size_t id, float time);
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		SceneManager *_sceneManager;
#endif

	public:
		// All functions are virtual so if you want to customize your Engine class
		// without hacking the core, you can heritate from :)
		Engine();
		virtual ~Engine();

		virtual bool _init();
		virtual bool _initInNewThread();
		virtual bool _release();
		virtual bool _releaseInNewThread();
		virtual bool _updateBegin();
		virtual bool _updateEnd();

		inline PrepareRenderThread *getPrepareThread()
		{
			return _prepareThread;
		}
		inline RenderThread *getRenderThread()
		{
			return _renderThread;
		}
		inline Engine *getMainThread()
		{
			return this;
		}
	};
}