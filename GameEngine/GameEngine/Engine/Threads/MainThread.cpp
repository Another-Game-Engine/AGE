#include "MainThread.hpp"
#include <Core/Engine.hh>
#include "QueueOwner.hpp"
#include "ThreadManager.hpp"
#include "RenderThread.hpp"
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/PrepareRenderThread.hpp>
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
				while (!_next->getQueue()->releaseCommandReadability(TMQ::HybridQueue::WaitType::Block))
				{
					if (getQueue()->getTaskQueue(taskQueue, TMQ::HybridQueue::NoWait))
					{
						SCOPE_profile_cpu_i("MainThread", "Execute tasks");
						workStart = std::chrono::high_resolution_clock::now();
						while (!taskQueue.empty())
						{
							auto task = taskQueue.front();
							assert(execute(task)); // we receive a task that we cannot handle
							taskQueue.pop();
							taskCounter--;
						}
						workEnd = std::chrono::high_resolution_clock::now();
						workCount += std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count();
					}
				}
			}
		}
		waitEnd = std::chrono::high_resolution_clock::now();

		bool hasTasks = getQueue()->getTaskQueue(taskQueue, TMQ::HybridQueue::NoWait);
		workStart = std::chrono::high_resolution_clock::now();
		if (hasTasks)
		{
			SCOPE_profile_cpu_i("MainThread", "Execute tasks");
			while (!taskQueue.empty())
			{
				auto task = taskQueue.front();
				auto success = execute(task); // we receive a task that we cannot handle
				AGE_ASSERT(success);
				taskQueue.pop();
				taskCounter--;
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
		_next->getQueue()->releaseCommandReadability(TMQ::HybridQueue::WaitType::Block);

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
		_next->getQueue()->reserveTo(std::this_thread::get_id().hash());
		if (!init())
			return false;
		return true;
	}

	bool MainThread::stop()
	{
		if (!release())
			return false;
		GetPrepareThread()->stop();
		return true;
	}

	void MainThread::setSceneAsActive(AScene *scene)
	{
		_activeScene = scene;
		GetPrepareThread()->getQueue()->emplaceCommand<Commands::MainToPrepare::SetCurrentScene>(scene);
	}

}