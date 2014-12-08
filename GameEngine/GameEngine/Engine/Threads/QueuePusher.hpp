#pragma once

namespace AGE
{
	class QueueOwner;

	// Command queue pusher are class who have a
	// pointer on the command queue of the next
	// thread in the pipeline (CommandQueueOwner)
	// A thread can be Owner and Pusher at the same
	// time, for example, Prepare render thread
	// push command to Render thread, so it's a
	// pusher, but main thread push command to it
	// so it's also an owner.
	class QueuePusher
	{
	public:
		QueuePusher();
		virtual ~QueuePusher();
		void linkToNext(QueueOwner *next);
	protected:
		QueueOwner *_next;

		friend class QueueOwner;
	};
}