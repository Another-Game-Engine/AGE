#pragma once

#include <thread>
#include <Utils/CommandQueue.hpp>
#include <Utils/ThreadName.hpp>
#include <iostream>
#include <string>

#include <Core/MainThreadCommand.hpp>

namespace AGE
{
	class ThreadQueue : public CommandQueue
	{
	protected:
		std::thread _thread;
		std::atomic_bool _run;
		bool _lastOfLoop;

		bool runFunction(Engine *engine)
		{
			bool run = true;
			_threadId = std::this_thread::get_id().hash();
			run = _initInNewThread();
			while (_run && run)
			{
				if (!commandQueueUpdate())
					break;
			}
			return _releaseInNewThread();
		}

	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		inline void setLastOfLoop(bool v)
		{
			_lastOfLoop = v;
		}

		ThreadQueue()
			: _lastOfLoop(false)
		{}

		virtual ~ThreadQueue()
		{

		}

		bool launch(Engine *engine, const std::string &name)
		{
			_name = name;
			_engine = engine;
			_run = true;
			auto res = init(engine);
			_thread = std::thread(&ThreadQueue::runFunction, std::ref(*this), engine);
#ifdef WIN32
			DWORD threadId = ::GetThreadId( static_cast<HANDLE>( _thread.native_handle() ) );
			SetThreadName(threadId, _name.c_str());
#endif
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