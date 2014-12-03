#include "CommandQueueOwner.hpp"
#include "CommandQueuePusher.hpp"

namespace AGE
{
	CommandQueuePusher::CommandQueuePusher()
		: _next(nullptr)
	{}

	CommandQueuePusher::~CommandQueuePusher()
	{}

	void CommandQueuePusher::LinkToNext(CommandQueueOwner *next)
	{
		assert(next == false);
		_next = next;
	}
}