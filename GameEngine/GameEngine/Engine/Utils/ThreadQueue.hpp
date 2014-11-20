#pragma once

#include <thread>
#include <Utils/CommandQueue.hpp>

namespace AGE
{
	class ThreadQueue : public CommandQueue
	{
	protected:
		std::thread _thread;

		virtual bool _update() = 0;
		virtual bool _init() = 0;
		virtual bool _initInNewThread() = 0;
		virtual bool _release() = 0;
		virtual bool _releaseInNewThread() = 0;
		std::atomic_bool _run;

		bool update(Engine *engine)
		{
			bool run = true;
			init(engine);
			run = _initInNewThread();
			while (_run && run)
			{
				run = commandQueueUpdate();
				if (_next)
					_next->releaseReadability();
			}
			return _releaseInNewThread();
		}

	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		ThreadQueue()
		{}

		virtual ~ThreadQueue()
		{

		}

		bool launch(Engine *engine)
		{
			_engine = engine;
			_run = true;
			auto res = _init();
			_thread = std::thread(&ThreadQueue::update, std::ref(*this), engine);
			return res;
		}

		void quit()
		{
			if (!_thread.joinable())
				return;
			_run = false;
			_thread.join();
			_release();
		}
	};
}