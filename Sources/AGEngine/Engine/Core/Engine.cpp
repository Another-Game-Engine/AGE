#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

#include "Engine.hh"

#include <Core/Timer.hh>

#include <Utils/Debug.hpp>
#include <Utils/Encoding.hpp>
#include <Utils/File.hpp>
#include <Utils/Directory.hpp>
#include <Utils/FileSystem.hpp>
#include <Utils/Path.hpp>
#include <Utils/Profiler.hpp>
#include <Utils/EngineBase.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/Tasks/BasicTasks.hpp>

#include <Utils/Age_microprofile.hpp>

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

#include <AssetManagement/AssetManager.hh>
#include <Core/SceneManager.hh>
#include <Context/IRenderContext.hh>
#include <Core/Inputs/Input.hh>
#include <Skinning/AnimationManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Entity/ArchetypeManager.hpp>

#endif

#ifdef AGE_ENABLE_IMGUI

#include <Core/Age_Imgui.hpp>

#endif

namespace AGE
{
	static BOOL CtrlHandler(DWORD fdwCtrlType)
	{
		switch (fdwCtrlType)
		{
			case CTRL_C_EVENT:
				// Disable CTRL-C signal.
				return TRUE;
			case CTRL_CLOSE_EVENT:
				// Exit properly if the console is closed.
				ExitAGE();
				return TRUE;
			case CTRL_BREAK_EVENT:
			case CTRL_LOGOFF_EVENT:
			case CTRL_SHUTDOWN_EVENT:
			default:
				// Pass other signals to the next handler.
				return FALSE;
		}
	}

	Engine::Engine(void)
		: EngineBase()
		//, _timer(nullptr)
	{

	}

	Engine::Engine(int argc, char *argv[])
		: EngineBase(argc, argv)
	{
		return;
	}

	Engine::Engine(const std::string &projectName, const std::string &password)
		: EngineBase(projectName, password)
	{
		return;
	}

	Engine::Engine(int argc, char *argv[], const std::string &projectName, const std::string &password)
		: EngineBase(argc, argv, projectName, password)
	{
		// Catch signals.
		SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(CtrlHandler), TRUE);
	}

	Engine::~Engine()
	{
		if (_initialized)
		{
			finalize();
		}
	}

	bool Engine::launch(std::function<bool()> &fn)
	{
		AGE_ASSERT(!_initialized && "Engine already initialized.");
		Age_microprofileInit();

		_timer = setInstance<Timer>();
		setInstance<FileSystem>();

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
		if (!getInstance<FileSystem>()->initialize(password.c_str()))
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
		getInstance<FileSystem>()->initialize(password.c_str());

		_initialized = true;


		auto configurationManager = setInstance<ConfigurationManager>("Configuration.json");
		configurationManager->loadFile();
		if (!configurationManager->getConfiguration<bool>("fullScreen"))
		{
			configurationManager->setConfiguration<bool>(std::string("fullScreen"), false);
		}
		if (!configurationManager->getConfiguration<int>("windowW"))
		{
			configurationManager->setConfiguration<int>(std::string("windowW"), 1280);
		}
		if (!configurationManager->getConfiguration<size_t>("frameCap"))
		{
			configurationManager->setConfiguration<size_t>(std::string("frameCap"), 0);
		}
		if (!configurationManager->getConfiguration<int>("windowH"))
		{
			configurationManager->setConfiguration<int>(std::string("windowH"), 720);
		}
		auto frameCap = configurationManager->getConfiguration<size_t>("frameCap");
		GetMainThread()->setFrameCap(frameCap->value);

		{
			auto futur = TMQ::TaskManager::emplaceRenderFutureTask<Tasks::Render::CreateRenderContext, bool>(this);
			auto success = futur.get();
			if (!success)
			{
				return false;
			}
		}
#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		setInstance<AGE::AssetsManager>();
		setInstance<AGE::AnimationManager>();
		setInstance<ArchetypeManager, IArchetypeManager>()->setLibraryFolder("../../Datas/Archetypes/");

		auto assetsFilePath = getInstance<ConfigurationManager>()->getConfiguration<std::string>("AssetsPath");
		if (assetsFilePath != nullptr)
		{
			getInstance<AGE::AssetsManager>()->setAssetsDirectory(assetsFilePath->getValue());
		}
#endif //USE_DEFAULT_ENGINE_CONFIGURATION

		if (!fn())
			return false;
		{
			auto futur = TMQ::TaskManager::emplaceRenderFutureTask<Tasks::Render::InitRenderPipelines, bool>(this);
			auto success = futur.get();
			if (!success)
			{
				return false;
			}
		}

		GetMainThread()->run();

		getInstance<ConfigurationManager>()->saveToFile();
		return true;
	}

	void Engine::finalize()
	{
		getInstance<FileSystem>()->finalize();
		AGE_ASSERT(_initialized && "Engine not initialized yet");
	}

	bool Engine::update()
	{
		SCOPE_profile_cpu_function("Main thread");

		bool res = false;

#ifdef AGE_ENABLE_IMGUI
		AGE::Imgui::getInstance()->startUpdate();
#endif
		_timer->update();

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		auto assetsManager = getInstance<AGE::AssetsManager>();
		assetsManager->update();
#endif //USE_DEFAULT_ENGINE_CONFIGURATION

			res = updateScenes(_timer->getElapsed() * _timeMultiplier);

		if (!res)
		{
			return false;
		}
		static float refreshStats = 0.0;
		refreshStats += _timer->getElapsed();

		TMQ::TaskManager::emplaceRenderTask<Commands::ToRender::Flush>(GetMainThread()->isRenderFrame());
		++frame;
		return true;
	}

	Engine *GetEngine()
	{
		AGE_ASSERT(AGE::EngineBase::g_engineInstance != nullptr);
		return (Engine*)AGE::EngineBase::g_engineInstance;
	}

	Engine *CreateEngine()
	{
		AGE_ASSERT(AGE::EngineBase::g_engineInstance == nullptr);
		AGE::EngineBase::g_engineInstance = GetThreadManager()->createEngine();
		return (Engine*)AGE::EngineBase::g_engineInstance;
	}

}