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

		workStart = std::chrono::high_resolution_clock::now();

		if (!_engine->update())
		{
			return false;
		}
		workEnd = std::chrono::high_resolution_clock::now();
		workCount += std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count();

		{
			SCOPE_profile_cpu_i("MainThread", "Release commands");
			waitStart = std::chrono::high_resolution_clock::now();
			{
				TMQ::MessageBase *task = nullptr;
				do {
					getQueue()->tryToGetTask(task, 10);
					if (task)
					{
						SCOPE_profile_cpu_i("MainThread", "Execute tasks");
						workStart = std::chrono::high_resolution_clock::now();
						auto result = execute(task);
						assert(result); // we receive a task that we cannot handle
					}
				} while (task != nullptr);
			}
		}

		workEnd = std::chrono::high_resolution_clock::now();
		workCount += std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count();

		GetThreadManager()->updateThreadStatistics(this->_id
			, workCount
			, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());

		return true;
	}

	bool MainThread::run()
	{
		_run = true;
		_insideRun = true;

		while (_run && _insideRun)
		{
			_run = update();
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