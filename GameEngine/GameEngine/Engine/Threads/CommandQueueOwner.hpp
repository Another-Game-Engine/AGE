#pragma once

#include <TMQ/queue.hpp>

namespace AGE
{
	class CommandQueuePusher;

	class CommandQueueOwner
	{
	public:
		inline TMQ::ReleasableQueue *getQueue() { return &_queue; }
	private:
		TMQ::ReleasableQueue _queue;

		friend class CommandQueuePusher;
	};
}