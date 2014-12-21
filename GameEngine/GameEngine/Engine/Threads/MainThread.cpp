#include "MainThread.hpp"
#include <Core/Engine.hh>
#include "QueueOwner.hpp"
#include "ThreadManager.hpp"
#include "RenderThread.hpp"
#include <Core/Tasks/Render.hpp>
#include <Threads/PrepareRenderThread.hpp>


namespace AGE
{
	bool MainThread::init()
	{
		// We register the main thread id
		_registerId();

#ifdef USE_IMGUI
		//registerCallback<MTC::FrameTime>([this](MTC::FrameTime& msg)
		//{
		//	//updateThreadStatistics(msg.id, msg.time);
		//});

		//registerCallback<MTC::ThreadRegistration>([this](MTC::ThreadRegistration& msg)
		//{
		//	//_threadsStatics.insert(std::make_pair(msg.id, Engine::ThreadStatistics()));
		//	//_threadsStatics[msg.id].name = msg.name;
		//});
#endif

		return true;
	}
	
	bool MainThread::release()
	{
		_insideRun = false;
		return true;
	}
	
	bool MainThread::update()
	{
		TMQ::PtrQueue taskQueue;

		std::chrono::system_clock::time_point waitStart;
		std::chrono::system_clock::time_point waitEnd;
		std::chrono::system_clock::time_point workStart;
		std::chrono::system_clock::time_point workEnd;

		waitStart = std::chrono::high_resolution_clock::now();
		if (!getQueue()->getTaskQueue(taskQueue, TMQ::HybridQueue::NoWait))
		{
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			if (!_engine->update())
				return false;
			while (!_next->getQueue()->releaseCommandReadability(TMQ::HybridQueue::WaitType::NoWait))
			{
				if (getQueue()->getTaskQueue(taskQueue, TMQ::HybridQueue::NoWait))
				{
					while (!taskQueue.empty())
					{
						auto task = taskQueue.front();
						assert(execute(task)); // we receive a task that we cannot handle
						taskQueue.pop();
					}
				}
			}
			workEnd = std::chrono::high_resolution_clock::now();
			GetThreadManager()->updateThreadStatistics(this->_id
				, std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count()
				, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());
		}
		else
		{
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			while (!taskQueue.empty())
			{
				auto task = taskQueue.front();
				assert(execute(task)); // we receive a task that we cannot handle
				taskQueue.pop();
			}
			workEnd = std::chrono::high_resolution_clock::now();
			GetThreadManager()->updateThreadStatistics(this->_id
				, std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count()
				, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());
		}

		return true;
	}

	bool MainThread::run()
	{
		_run = true;
		_insideRun.store(true);
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

	std::weak_ptr<AGE::Engine> MainThread::createEngine()
	{
		static bool unique = true;
		if (unique)
		{
			_engine = std::make_shared<AGE::Engine>();
			auto engine = std::weak_ptr<AGE::Engine>(_engine);
			auto futur = GetRenderThread()->getQueue()->emplaceFutureTask<Tasks::Render::CreateRenderContext, bool>(engine);
			auto success = futur.get();
			assert(success);
		}
		unique = false;
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
	}

}