#include "MainThread.hpp"
#include <Core/Engine.hh>
#include "CommandQueueOwner.hpp"

namespace AGE
{
	bool MainThread::init()
	{
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
		// We register the main thread id
		_registerId();
	}
	
	MainThread::~MainThread()
	{}

	AGE::Engine *MainThread::createEngine()
	{
		_engines.push_back(std::make_unique<AGE::Engine>());
		return _engines.back().get();
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

}