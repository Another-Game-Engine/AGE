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

		virtual bool _init() = 0;
		virtual bool _initInNewThread() = 0;
		virtual bool _release() = 0;
		virtual bool _releaseInNewThread() = 0;
		virtual bool _updateBegin() = 0;
		virtual bool _updateEnd() = 0;

		std::atomic_bool _run;
		std::string _name;
		std::chrono::system_clock::time_point _time;
		bool _lastOfLoop;

		bool update(Engine *engine)
		{
			bool run = true;
			init(engine);
			run = _initInNewThread();
			while (_run && run)
			{
				run = updateBegin();
				if (!run)
					continue;
				if (!commandQueueUpdate())
					continue;
				run = updateEnd();
				if (_next && !_lastOfLoop)
					_next->releaseReadability();
				if (!run)
					continue;
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

		bool updateBegin()
		{
			_time = std::chrono::system_clock::now();
			return _updateBegin();
		}

		bool updateEnd()
		{
			bool res =_updateEnd();
			if (!res)
				return res;
			auto elapsed = std::chrono::system_clock::now() - _time;
			if (_next)
				_next->emplace<AGE::MTC::FrameTime>(_name, elapsed.count());
			return res;
		}

		bool launch(Engine *engine, const std::string &name)
		{
			_name = name;
			_engine = engine;
			_run = true;
			auto res = _init();
			_thread = std::thread(&ThreadQueue::update, std::ref(*this), engine);
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