#include "Engine.hh"

#include <Core/Timer.hh>

#include <Utils/Debug.hpp>
#include <Utils/Encoding.hpp>
#include <Utils/File.hpp>
#include <Utils/Directory.hpp>
#include <Utils/FileSystem.hpp>
#include <Utils/Path.hpp>
#include <Utils/Profiler.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/Tasks/BasicTasks.hpp>

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION

#include <AssetManagement/AssetManager.hh>
#include <Core/SceneManager.hh>
#include <Context/IRenderContext.hh>
#include <Core/Inputs/Input.hh>
#include <Skinning/AnimationManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Entities/ArchetypeManager.hpp>

#endif

#ifdef AGE_ENABLE_IMGUI

#include <Utils/Age_Imgui.hpp>

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
		: Engine(0, nullptr)
		//, _timer(nullptr)
	{

	}

	Engine::Engine(int argc, char *argv[])
		: Engine(argc, argv, "AgeGameEngine", "")
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

	std::size_t Engine::getFrameNumber(void) const
	{
		return frame;
	}

	bool Engine::launch(std::function<bool()> &fn)
	{
		AGE_ASSERT(!_initialized && "Engine already initialized.");

		{
			auto futur = GetRenderThread()->getQueue()->emplaceFutureTask<Tasks::Render::CreateRenderContext, bool>(this);
			auto success = futur.get();
			if (!success)
			{
				return false;
			}
		}

		_timer = setInstance<Timer>();

		setInstance<FileSystem>();

#ifdef USE_DEFAULT_ENGINE_CONFIGURATION
		setInstance<AGE::AssetsManager>();
		setInstance<AGE::AnimationManager>();
		setInstance<ConfigurationManager>("../../Configuration.json");
		setInstance<ArchetypeManager>()->setLibraryFolder("../../Archetypes/");

		getInstance<ConfigurationManager>()->loadFile();
		auto assetsFilePath = getInstance<ConfigurationManager>()->getConfiguration<std::string>("AssetsPath");
		if (assetsFilePath != nullptr)
		{
			getInstance<AGE::AssetsManager>()->setAssetsDirectory(assetsFilePath->getValue());
		}
		else
		{
			AGE_BREAK(); // "Assets file path is missing. Add it in configuration.json"
		}
#endif //USE_DEFAULT_ENGINE_CONFIGURATION

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

		if (!fn())
			return false;
		{
			auto futur = GetRenderThread()->getQueue()->emplaceFutureTask<Tasks::Render::InitRenderPipelines, bool>(this);
			auto success = futur.get();
			if (!success)
			{
				return false;
			}
		}

		_initialized = true;

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
		auto &stats = GetThreadManager()->getStatistics();
		if (refreshStats >= 1.0)
		{
			for (auto i = (std::size_t)Thread::Main; i < Thread::hardwareConcurency(); ++i)
			{
				auto &e = stats[i];
				e.waitCopy[e.secondCounter] = e.averageWaitTime;
				e.workCopy[e.secondCounter] = e.averageWorkTime;
				e.totalCopy[e.secondCounter] = e.averageWaitTime + e.averageWorkTime;
				++e.secondCounter;
				if (e.secondCounter >= e.waitCopy.size())
					e.secondCounter = 0;
			}
			refreshStats = 0.0;
		}
		for (auto i = (std::size_t)Thread::Main; i < Thread::hardwareConcurency(); ++i)
		{
			auto &e = stats[i];
			e.averageWaitTimeCopy = e.averageWaitTime;
			e.averageWorkTimeCopy = e.averageWorkTime;
		}

		if (_displayThreadsStatistics)
		{
			_renderThreadsStatistics();
		}
		if (_displayFps)
		{
			_renderFpsStatitstics();
		}
#ifdef AGE_ENABLE_IMGUI
		ImGui::Render();
#endif
		GetPrepareThread()->getQueue()->emplaceCommand<Commands::ToRender::Flush>();
		++frame;
		return true;
	}

	void Engine::_renderThreadsStatistics()
	{
#ifdef AGE_ENABLE_IMGUI
		if (ImGui::Begin("Threads statistics", (bool*)0, ImVec2(0, 0), -1.0f, ImGuiWindowFlags_AlwaysAutoResize))
		{
			auto &stats = GetThreadManager()->getStatistics();
			for (auto i = (std::size_t)Thread::Main; i <= Thread::Render; ++i)
			{
				auto &e = stats[i];
				e.averageWaitTimeCopy = e.averageWaitTime;
				e.averageWorkTimeCopy = e.averageWorkTime;
				if (ImGui::CollapsingHeader(e.name.c_str(), (const char*)0, true, true))
				{
					if (e.averageWaitTime + e.averageWorkTime != 0)
						ImGui::Text(std::string("Total : " + std::to_string(e.averageWaitTimeCopy + e.averageWorkTimeCopy) + " ms.").c_str());
					ImGui::Text(std::string("Work : " + std::to_string((float)e.averageWorkTimeCopy) + " ms.").c_str());
					ImGui::Text(std::string("Wait : " + std::to_string((float)e.averageWaitTimeCopy) + " ms.").c_str());
					if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
						ImGui::Text(std::string("FPS : " + std::to_string((int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy))) + ".").c_str());
					ImGui::PlotLines("Frame Times", e.totalCopy.data(), (int)e.totalCopy.size(), (int)e.secondCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(200, 30));
					if (ImGui::TreeNode((void*)(&e), "Details"))
					{
						ImGui::PlotLines("Wait Times", e.waitCopy.data(), (int)e.waitCopy.size(), (int)e.secondCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(200, 30));
						ImGui::TreePop();
					}
				}
			}
			if (ImGui::CollapsingHeader("Workers"))
			{
				for (auto i = (std::size_t)Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
				{
					auto &e = stats[i];
					ImGui::Text("%s", e.name.c_str());
					ImGui::PlotLines("", e.workCopy.data(), (int)e.workCopy.size(), (int)e.secondCounter, e.name.c_str(), 0.0f, 40.0f, ImVec2(200, 30));
					ImGui::Separator();
				}
			}
		}
		ImGui::End();
#endif
	}

	void Engine::_renderFpsStatitstics()
	{
#ifdef AGE_ENABLE_IMGUI
		if (!ImGui::Begin("Example: Fixed OverlayFPS OVERLAY", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
		}
		else
		{
			ImGui::SetWindowPos(ImVec2(10, 10));
			{
				auto &e = GetThreadManager()->getStatistics()[Thread::Main];
				if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
					ImGui::Text("Main : %i fps", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			}
			{
				auto &e = GetThreadManager()->getStatistics()[Thread::PrepareRender];
				if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
					ImGui::Text("Prepare : %i fps", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			}
			{
				auto &e = GetThreadManager()->getStatistics()[Thread::Render];
				if (e.averageWaitTimeCopy + e.averageWorkTimeCopy > 0)
					ImGui::Text("Render : %i fps", (int)(1000 / (e.averageWaitTimeCopy + e.averageWorkTimeCopy)));
			}
			ImGui::End();
		}
#endif
	}

	Engine *GetEngine()
	{
		return GetThreadManager()->getEngine();
	}

	Engine *CreateEngine()
	{
		return GetThreadManager()->createEngine();
	}

}