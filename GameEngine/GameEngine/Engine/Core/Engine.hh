#pragma once

#include <Configuration.hpp>
#include "Utils/DependenciesInjector.hpp"
#include <Core/SceneManager.hh>
#include <Utils/Singleton.hh>

namespace AGE
{
	class Timer;

	class Engine : public DependenciesInjector
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		, public SceneManager
#endif
	{
	public:
		bool launch(std::function<bool()> &fn);
		bool update();
		void finalize();
		~Engine();

		std::size_t getNumberOfArguments(void) const;
		const std::string &getArgument(std::size_t num) const;

		const std::string &getProjectName(void) const;
		const std::string &getPassword(void) const;

		const std::string &getApplicationPath(void) const;
		const std::string &getDataPath(void) const;
		const std::string &getHomePath(void) const;
		const std::string &getSavePath(void) const;
		const std::string &getCachePath(void) const;

		std::size_t getFrameNumber(void) const;
		inline float getTimeMultiplier() const { return _timeMultiplier; }
		inline void setTimeMultiplier(float multiplier) { if (multiplier < 0.0f) return; _timeMultiplier = multiplier; }
	private:
		friend class Singleton < Engine > ;

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

		Timer *_timer;
		bool _initialized = false;
		int numberOfArguments;
		std::vector<std::string> arguments;
		const std::string projectName;
		const std::string password;
		std::string applicationPath;
		std::string dataPath;
		std::string homePath;
		std::string savePath;
		std::string cachePath;
		std::size_t frame = 0;
		float _timeMultiplier = 1.0f;
	};

	Engine *CreateEngine();
	Engine *GetEngine();
}