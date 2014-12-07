#pragma once

#include "Thread.hpp"
#include "CommandQueueOwner.hpp"

namespace AGE
{
	class ThreadManager;

	class TaskThread : public Thread, public QueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		virtual bool launch();
		virtual bool stop();
		bool update();
	private:
		TaskThread(Thread::ThreadType type);
		virtual ~TaskThread();
		TaskThread() = delete;
		TaskThread(const TaskThread &) = delete;
		TaskThread(TaskThread &&) = delete;
		TaskThread &operator=(const TaskThread &) = delete;
		TaskThread &operator=(TaskThread &&) = delete;
		friend class ThreadManager;

		std::thread _threadHandle;
		std::atomic_bool _run;
	};
}