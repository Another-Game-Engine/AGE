#include "QueueOwner.hpp"
#include "QueuePusher.hpp"

namespace AGE
{
	QueuePusher::QueuePusher()
		: _next(nullptr)
	{}

	QueuePusher::~QueuePusher()
	{}

	void QueuePusher::linkToNext(QueueOwner *next)
	{
		assert(next != nullptr);
		_next = next;
	}
}