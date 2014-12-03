#pragma once

#include <TMQ/queue.hpp>

namespace AGE
{
	class CommandQueuePusher;

	class CommandQueueOwner
	{
	public:
		inline TMQ::ReleasableQueue *getCommandQueue() { return &_queue; }
	protected:
		TMQ::ReleasableQueue _queue;

		friend class CommandQueuePusher;
	};
}