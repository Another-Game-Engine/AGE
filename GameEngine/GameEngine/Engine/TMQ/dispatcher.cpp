#include "dispatcher.hpp"
#include "message.hpp"


using namespace TMQ;

void Dispatcher::waitAndDispatch()
{
	//TMQ::PtrQueue q;
	//_queue->getReadableQueue(q);
	//while (!q.empty())
	//{
	//	auto message = q.front();
	//	dispatch(message);
	//	q.pop();
	//}
}

bool Dispatcher::dispatch(MessageBase* &msg)
{
	return false;
}

Dispatcher::Dispatcher(Dispatcher &&o) :
_queue(o._queue)
, _chained(o._chained)
{
	o._chained = false;
}

Dispatcher::Dispatcher(TMQ::Queue *queue)
	: _queue(queue)
	, _chained(false)
{}

Dispatcher::~Dispatcher()
{
	if (!_chained)
	{
		waitAndDispatch();
	}
}