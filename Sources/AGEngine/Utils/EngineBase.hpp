#pragma once

#include "DependenciesInjector.hpp"

namespace AGE
{
	class EngineBase : public DependenciesInjector
	{
	public:
		static EngineBase *g_engineInstance;

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

		virtual ~EngineBase();
		EngineBase(void);
		EngineBase(int argc, char *argv[]);
		EngineBase(const std::string &projectName, const std::string &password);
		EngineBase(int argc, char *argv[], const std::string &projectName, const std::string &password);
		EngineBase(const EngineBase &other) = delete;
		EngineBase(EngineBase &&other) = delete;
		EngineBase &operator=(const EngineBase &other) = delete;
		EngineBase &operator=(EngineBase &&other) = delete;

	protected:
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
}