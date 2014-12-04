#include "MainThread.hpp"
#include <Core/Engine.hh>
#include "CommandQueueOwner.hpp"
#include "ThreadManager.hpp"
#include "RenderThread.hpp"

namespace AGE
{
	bool MainThread::init()
	{
		// We register the main thread id
		_registerId();
		return true;
	}
	
	bool MainThread::release()
	{
		return true;
	}
	
	bool MainThread::update()
	{
		_engine->update();

		TMQ::PtrQueue queue;

		bool released = false;

		while (!released || !queue.empty())
		{
			if (!queue.empty())
			{
				auto task = queue.front();
				assert(executeTask(task)); // we receive a task that we cannot handle
				queue.pop();
			}
			else
			{
				getTaskQueue()->getReadableQueue(queue);
			}
			if (!released)
			{
				released = _next->getCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait);
			}
		}

		return true;
	}

	bool MainThread::run()
	{
		_run = true;
		while (_run)
		{
			if (!update())
				return false;
		}
	}

	MainThread::MainThread()
		: Thread(Thread::ThreadType::Main)
		, _engine(nullptr)
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
			auto futur = GetRenderThread()->getTaskQueue()->emplaceFuture<MainThreadToRenderThread::CreateRenderContext, bool>(engine);
			auto success = futur.get();
			assert(success);
		}
		unique = false;
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
		return true;
	}

}