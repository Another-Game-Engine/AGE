#include <stdexcept>
#include <cassert>

#include "Platform.hpp"
#include "Engine.hpp"
#include "Directory.hpp"
#include "Encoding.hpp"
#include "FileSystem.hpp"
#include "Path.hpp"
#include "Debug.hpp"
#include "FileSystemThread.hpp"

namespace Age
{
	namespace Engine
	{
		Engine::Engine(void)
			: Engine(0, nullptr)
		{
			return;
		}

		Engine::Engine(int argc, char *argv[])
			: Engine(argc, argv, "DefaultProjectName", "")
		{
			return;
		}

		Engine::Engine(const std::string &projectName, const std::string &password)
			: Engine(0, nullptr, projectName, password)
		{
			return;
		}

		Engine::Engine(int argc, char *argv[], const std::string &projectName, const std::string &password)
			: numberOfArguments(argc), projectName(projectName), password(password)
		{
			arguments.reserve(argc);
			for (int index = 0; index < argc; ++index)
			{
				arguments.push_back(argv[index]);
			}
		}

		Engine::~Engine(void)
		{
			if (initialized)
			{
				finalize();
			}
		}

		std::size_t Engine::getNumberOfArguments(void) const
		{
			return numberOfArguments;
		}

		const std::string &Engine::getArgument(std::size_t num) const
		{
			assert(num < arguments.size() && "Invalid argument");
			return arguments[num];
		}

		const std::string &Engine::getProjectName(void) const
		{
			return projectName;
		}

		const std::string &Engine::getPassword(void) const
		{
			return password;
		}

		const std::string &Engine::getApplicationPath(void) const
		{
			return applicationPath;
		}

		const std::string &Engine::getDataPath(void) const
		{
			return dataPath;
		}

		const std::string &Engine::getHomePath(void) const
		{
			return homePath;
		}

		const std::string &Engine::getSavePath(void) const
		{
			return savePath;
		}

		const std::string &Engine::getCachePath(void) const
		{
			return cachePath;
		}

		std::shared_ptr<FileSystem> Engine::getFileSystem(void) const
		{
			return fileSystem;
		}

		std::size_t Engine::getFrameNumber(void) const
		{
			return frame;
		}

		void Engine::initialize(void)
		{
			AGE_ASSERT(!initialized && "Engine already initialized");
			wchar_t buf[BufferSize];
			if (GetModuleFileNameW(nullptr, buf, sizeof(buf)))
			{
				buf[sizeof(buf) / sizeof(wchar_t) - 1] = '\0';
				applicationPath = Encoding::UnicodeToUtf8(buf);
				std::size_t startPos = 0;
				const std::string from("\\");
				const std::string to("/");
				while ((startPos = applicationPath.find(from, startPos)) != std::string::npos)
				{
					applicationPath.replace(startPos, from.length(), to);
					startPos += to.length();
				}
				if (!applicationPath.empty())
				{
					applicationPath[0] = toupper(applicationPath[0]);
				}
				const std::size_t pos = applicationPath.rfind('/');
				if (pos != std::string::npos)
				{
					applicationPath.erase(pos + 1, applicationPath.size() - pos - 1);
				}
				if (!Directory::ChangeDirectory(applicationPath.c_str()))
				{
					throw std::exception("Impossible to change directory");
				}
			}
			else
			{
				throw std::exception("GetModuleFileNameW() failed");
			}
			homePath = Directory::GetHomeDirectory();
			homePath += '/';
			fileSystem = std::make_shared<FileSystem>();
			for (std::size_t index = 1, size = arguments.size(); index < size; ++index)
			{
				const char *argument = arguments[index].c_str();
				if (*argument == '-')
				{
					while (*argument && *argument == '-')
					{
						++argument;
					}
					if (!std::strcmp(argument, "dataPath") && index < size - 1)
					{
						dataPath = arguments[++index];
						break;
					}
				}
			}
			if (!projectName.empty())
			{
				savePath = homePath + projectName + "/";
				Directory::CreateDirectory(savePath.c_str());
			}
			else
			{
				savePath = applicationPath;
			}
			dataPath = Path::PathName((applicationPath + dataPath + "data/").c_str());
			Directory::CreateDirectory(dataPath.c_str());
			if (!fileSystem->initialize(password.c_str()))
			{
				throw std::exception("Impossible to initialize FileSystem");
			}
			if (!projectName.empty())
			{
				cachePath = savePath;
			}
			else
			{
				cachePath = dataPath;
			}
			fileSystem->initialize(password.c_str());
			initialized = true;
		}

		int Engine::run(void)
		{
			AGE_ASSERT(initialized && "Engine not initialized yet");
			while (true)
			{
				++frame;
				// TODO: Remove this part (test purpose)
				File file;
				file.open(getArgument(0).c_str(), "r");
				if (file.isOpened())
				{
					std::string line;
					for (line = file.readLine(); !file.endOfFile(); line = file.readLine())
					{
						AGE_LOG(line);
					}
					file.close();
				}
				// END_TODO
			}
			return AGE_SUCCESS;
		}

		void Engine::finalize(void)
		{
			AGE_ASSERT(initialized && "Engine not initialized yet");
			fileSystem.reset();
		}
	}
}