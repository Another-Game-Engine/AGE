#pragma once

#include "Thread.hpp"

namespace Age
{
	namespace Engine
	{
		Thread::~Thread(void)
		{
			resume();
			stop();
		}

		void Thread::run(void)
		{
			if (!running)
			{
				running = true;
				thread = std::thread([this](void)
				{
					Thread::Handler(this);
				});
			}
		}

		void Thread::stop(void)
		{
			if (running)
			{
				running = false;
				resume();
				if (thread.joinable())
				{
					thread.join();
				}
			}
		}

		void Thread::resume(void)
		{
			if (waiting)
			{
				std::unique_lock<std::mutex> lock(mutex);
				waiting = false;
				condition.notify_all();
			}
		}

		void Thread::pause(void)
		{
			if (running && !waiting)
			{
				std::unique_lock<std::mutex> lock(mutex);
				waiting = true;
				while (waiting)
				{
					condition.wait(lock);
				}
			}
		}

		bool Thread::isRunning(void) const
		{
			return running;
		}

		bool Thread::isWaiting(void) const
		{
			return waiting;
		}

		void Thread::lock(void)
		{
			locker.lock();
		}

		void Thread::unlock(void)
		{
			locker.unlock();
		}

		bool Thread::isLocked(void) const
		{
			return locker.isLocked();
		}

		void Thread::Handler(Thread *thread)
		{
			if (thread->running)
			{
				thread->process();
				thread->running = false;
			}
		}
	}
}
