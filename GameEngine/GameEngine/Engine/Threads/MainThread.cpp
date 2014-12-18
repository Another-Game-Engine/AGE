#include "MainThread.hpp"
#include <Core/Engine.hh>
#include "QueueOwner.hpp"
#include "ThreadManager.hpp"
#include "RenderThread.hpp"
#include <Core/Tasks/Render.hpp>

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

		if (!getQueue()->getTaskQueue(taskQueue, TMQ::HybridQueue::NoWait))
		{
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
		}
		else
		{
			while (!taskQueue.empty())
			{
				auto task = taskQueue.front();
				assert(execute(task)); // we receive a task that we cannot handle
				taskQueue.pop();
			}
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
		//ExitAGE();
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
		bool unique = true;
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
		return true;
	}

	void MainThread::setSceneAsActive(AScene *scene)
	{
		_activeScene = scene;
	}

}