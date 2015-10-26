#include "MainThread.hpp"
#include <Core/Engine.hh>
#include "QueueOwner.hpp"
#include "ThreadManager.hpp"
#include "RenderThread.hpp"
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Utils/Debug.hpp>
#include <Utils/Profiler.hpp>


namespace AGE
{
	bool MainThread::init()
	{
		// We register the main thread id
		_registerId();
		registerCallback<Tasks::Basic::VoidFunction>([this](Tasks::Basic::VoidFunction &msg){
			if (msg.function)
				msg.function();
		});
		_isRenderFrame = true;
		_frameCounter = 0;
		return true;
	}

	bool MainThread::release()
	{
		_insideRun = false;
		return true;
	}

	bool MainThread::update()
	{
		SCOPE_profile_cpu_function("Main thread");
		std::chrono::system_clock::time_point waitStart;
		std::chrono::system_clock::time_point waitEnd;
		std::chrono::system_clock::time_point workStart;
		std::chrono::system_clock::time_point workEnd;
		std::size_t workCount = 0;
		std::size_t waitCount = 0;

		workStart = std::chrono::high_resolution_clock::now();

		if (_frameCounter - GetRenderThread()->getCurrentFrameCount() > 2)
		{
			_isRenderFrame = false;
		}
		else
		{
			_isRenderFrame = true;
		}

		if (!_engine->update())
		{
			return false;
		}

		{
			SCOPE_profile_cpu_i("MainThread", "Execute tasks");
			{
				TMQ::MessageBase *task = nullptr;
				while (TMQ::TaskManager::MainThreadGetTask(task))
				{
					SCOPE_profile_cpu_i("MainThread", "Execute task");
					auto result = execute(task);
					assert(result); // we receive a task that we cannot handle
				}
			}
		}

		workEnd = std::chrono::high_resolution_clock::now();
		workCount = std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count();
		workCount -= waitCount;

		if (_isRenderFrame)
		{
			++_frameCounter;
		}

		return true;
	}

	bool MainThread::tryToStealTasks()
	{
		SCOPE_profile_cpu_i("MainThread", "Steal tasks");
		{
			TMQ::MessageBase *task = nullptr;
			if (TMQ::TaskManager::MainThreadGetTask(task))
			{
				SCOPE_profile_cpu_i("MainThread", "Execute task");
				auto result = execute(task);
				assert(result); // we receive a task that we cannot handle
				return true;
			}
		}
		return false;
	}

	bool MainThread::run()
	{
		_run = true;
		_insideRun = true;

		std::chrono::system_clock::time_point start;
		while (_run && _insideRun)
		{
			start = std::chrono::high_resolution_clock::now();
			_run = update();
			auto stop = std::chrono::high_resolution_clock::now();
			while (std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() < 16000)
			{
				std::this_thread::yield();
				stop = std::chrono::high_resolution_clock::now();
			}
		}
		ExitAGE();
		return true;
	}

	MainThread::MainThread()
		: Thread(Thread::ThreadType::Main)
		, _engine(nullptr)
		, _activeScene(nullptr)
	{
	}

	MainThread::~MainThread()
	{}

	AGE::Engine *MainThread::createEngine()
	{
		static bool unique = true;
		if (unique)
		{
			Singleton<Engine>::setInstance();
			_engine = Singleton<Engine>::getInstance();
		}
		unique = false;
		return _engine;
	}

	AGE::Engine *MainThread::getEngine()
	{
		AGE_ASSERT(_engine != nullptr && "Engine has not been created. Use 'CreateEngine()'.");
		return _engine;
	}

	bool MainThread::launch()
	{
		if (!init())
			return false;
		return true;
	}

	bool MainThread::stop()
	{
		if (!release())
			return false;
		GetRenderThread()->stop();
		return true;
	}
}