#pragma once

#include <TMQ/queue.hpp>

namespace AGE
{
	class TaskQueueOwner
	{
	public:
		inline TMQ::ImmediateQueue *getTaskQueue() { return &_queue; }
	private:
		TMQ::ImmediateQueue _queue;
	};
}