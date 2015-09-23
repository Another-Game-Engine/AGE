#pragma once

#include <Configuration.hpp>

#include <Utils/Singleton.hh>
#include "Utils/DependenciesInjector.hpp"
#include "Utils/EngineBase.hpp"

#include <Core/SceneManager.hh>

namespace AGE
{
	class Timer;

	class Engine : public EngineBase
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		, public SceneManager
#endif
	{
	public:
		bool launch(std::function<bool()> &fn);
		bool update();
		void finalize();
		~Engine();


		inline void displayFps(bool tof) { _displayFps = tof; }
		inline void displayThreadsStatistics(bool tof) { _displayThreadsStatistics = tof; }

		Engine(void);
		Engine(int argc, char *argv[]);
		Engine(const std::string &projectName, const std::string &password);
		Engine(int argc, char *argv[], const std::string &projectName, const std::string &password);
		Engine(const Engine &other) = delete;
		Engine(Engine &&other) = delete;
		Engine &operator=(const Engine &other) = delete;
		Engine &operator=(Engine &&other) = delete;

	private:
		static const std::size_t BufferSize = 1024;

		void _renderThreadsStatistics();
		void _renderFpsStatitstics();

		Timer *_timer;
		bool _initialized = false;

		bool _displayFps = true;
		bool _displayThreadsStatistics = true;
	};

	Engine *CreateEngine();
	Engine *GetEngine();
}