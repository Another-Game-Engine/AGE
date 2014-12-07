#include "QueueOwner.hpp"
#include "QueuePusher.hpp"

namespace AGE
{
	QueuePusher::QueuePusher()
		: _next(nullptr)
	{}

	QueuePusher::~QueuePusher()
	{}

	void QueuePusher::LinkToNext(QueueOwner *next)
	{
		assert(next == false);
		_next = next;
	}
}