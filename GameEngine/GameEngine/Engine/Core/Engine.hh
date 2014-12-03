#pragma once

#include <Configuration.hpp>
#include "Utils/DependenciesInjector.hpp"
#include <Utils/ThreadQueue.hpp>
#include <array>
#include <map>
#include <Core/SceneManager.hh>

namespace AGE
{
	class OldPrepareRenderThread;
	class OldRenderThread;
	class Timer;

	class Engine : public DependenciesInjector
		, public CommandQueue
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		, public SceneManager
#endif
	{
	protected:
		Engine(Engine const &);
		Engine &operator=(Engine const &);

		OldPrepareRenderThread *_prepareThread;
		OldRenderThread *_renderThread;
		Timer *_timer;

		struct ThreadStatistics
		{
			std::string name;
			float averageTime;
			unsigned char frameCounter;
			std::array < float, 64 > frames;
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

		void quit()
		{
			_release();
		}

		inline OldPrepareRenderThread *getPrepareThread()
		{
			return _prepareThread;
		}
		inline OldRenderThread *getRenderThread()
		{
			return _renderThread;
		}
		inline Engine *getMainThread()
		{
			return this;
		}
	};
}