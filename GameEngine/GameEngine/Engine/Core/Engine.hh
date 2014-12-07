#pragma once

#include <Configuration.hpp>
#include "Utils/DependenciesInjector.hpp"
#include <array>
#include <map>
#include <Core/SceneManager.hh>

namespace AGE
{
	class Timer;

	class Engine : public DependenciesInjector
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		, public SceneManager
#endif
	{
	protected:
		Engine(Engine const &);
		Engine &operator=(Engine const &);

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
		Engine();
		virtual ~Engine();

		bool launch(std::function<bool()> &fn);
		bool update();
	};
}