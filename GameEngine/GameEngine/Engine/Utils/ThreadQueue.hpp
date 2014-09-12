#pragma once

#include <thread>
#include <tmq/templateDispatcher.hpp>
#include <tmq/queue.hpp>
#include <Core/Engine.hh>
namespace AGE
{
	class ThreadQueue
	{
	private:
		std::thread _thread;
	protected:
		TMQ::Queue _commandQueue;
		virtual bool _update() = 0;
		virtual bool _init() = 0;
		virtual bool _initInNewThread() = 0;
		virtual bool _release() = 0;
		virtual bool _releaseInNewThread() = 0;
		Engine *_engine;
		std::atomic_bool _run;

		bool update()
		{
			bool run = true;
			run = _initInNewThread();
			while (_run && run)
			{
				run = _update();
			}
			return _releaseInNewThread();
		}

	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		ThreadQueue()
			: _engine(nullptr)
		{}

		virtual ~ThreadQueue()
		{
			if (_thread.joinable())
				quit();
		}

		bool launch(Engine *engine)
		{
			_run = true;
			_engine = engine;
			assert(_engine != nullptr);
			auto res = _init();
			_commandQueue.launch();
			_thread = std::thread(&ThreadQueue::update, std::ref(*this));
			return res;
		}

		void quit()
		{
			_run = false;
			_commandQueue.releaseReadability();
			_thread.join();
			_release();
		}

		TMQ::Queue &getCommandQueue()
		{
			return _commandQueue;
		}
	};
}