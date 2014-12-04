#pragma once

#include "Thread.hpp"
#include "TaskQueueOwner.hpp"

namespace AGE
{
	class ThreadManager;

	class TaskThread : public Thread, public TaskQueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		virtual bool launch();
		virtual bool stop();
	private:
		TaskThread(Thread::ThreadType type);
		virtual ~TaskThread();
		TaskThread() = delete;
		TaskThread(const TaskThread &) = delete;
		TaskThread(TaskThread &&) = delete;
		TaskThread &operator=(const TaskThread &) = delete;
		TaskThread &operator=(TaskThread &&) = delete;
		friend class ThreadManager;
	};
}