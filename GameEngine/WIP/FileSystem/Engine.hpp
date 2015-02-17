#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Singleton.hpp"

namespace Age
{
	namespace Engine
	{
		class FileSystem;

		class Engine final : public Singleton<Engine>
		{
			friend Access;

		public:
			~Engine(void);

			std::size_t getNumberOfArguments(void) const;
			const std::string &getArgument(std::size_t num) const;

			const std::string &getProjectName(void) const;
			const std::string &getPassword(void) const;

			const std::string &getApplicationPath(void) const;
			const std::string &getDataPath(void) const;
			const std::string &getHomePath(void) const;
			const std::string &getSavePath(void) const;
			const std::string &getCachePath(void) const;

			std::shared_ptr<FileSystem> getFileSystem(void) const;

			std::size_t getFrameNumber(void) const;

			void initialize(void);
			int run(void);
			void finalize(void);

		private:
			static const std::size_t BufferSize = 1024;

			bool initialized = false;
			int numberOfArguments;
			std::vector<std::string> arguments;
			const std::string projectName;
			const std::string password;
			std::shared_ptr<FileSystem> fileSystem;
			std::string applicationPath;
			std::string dataPath;
			std::string homePath;
			std::string savePath;
			std::string cachePath;
			std::size_t frame = 0;

			Engine(void);
			Engine(int argc, char *argv[]);
			Engine(const std::string &projectName, const std::string &password);
			Engine(int argc, char *argv[], const std::string &projectName, const std::string &password);
			Engine(const Engine &other) = delete;
			Engine(Engine &&other) = delete;
			Engine &operator=(const Engine &other) = delete;
			Engine &operator=(Engine &&other) = delete;
		};
	}
}