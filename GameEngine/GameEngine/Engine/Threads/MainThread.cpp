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
		for (auto &e : _engines)
		{
			//e->update();
		}

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

	MainThread::MainThread()
		: Thread(Thread::ThreadType::Main)
		, _activeEngine(nullptr)
	{
	}
	
	MainThread::~MainThread()
	{}

	AGE::Engine *MainThread::createEngine()
	{
		_engines.push_back(std::make_unique<AGE::Engine>());
		auto engine = _engines.back().get();
		auto futur = GetRenderThread()->getTaskQueue()->emplaceFuture<MainThreadToRenderThread::CreateRenderContext, bool>(engine);
		assert(futur.get());
		return engine;
	}

	void MainThread::destroyEngine(AGE::Engine *engine)
	{
		std::size_t i = 0;
		for (std::size_t i = 0; i < _engines.size(); ++i)
		{
			if (_engines[i].get() == engine)
				break;
		}
		assert(i < _engines.size());
		//_engines[i]->quit();
		if (i != _engines.size() - 1)
		{
			std::swap(_engines[i], _engines.back());
		}
		_engines.pop_back();
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