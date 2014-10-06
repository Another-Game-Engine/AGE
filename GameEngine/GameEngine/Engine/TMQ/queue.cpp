#include "queue.hpp"
#include "dispatcher.hpp"

using namespace TMQ;

PtrQueue::PtrQueue(std::size_t chunkSize)
: _chunkSize(chunkSize)
, _data(nullptr)
, _cursor(0)
, _size(0)
, _to(0)
{
}


PtrQueue& PtrQueue::operator=(PtrQueue &&o)
{
	std::swap(_data, o._data);
	_chunkSize = std::move(o._chunkSize);
	_cursor = std::move(o._cursor);
	_to = std::move(o._to);
	std::swap(_size, o._size);
	o.clear();
	return *this;
}

PtrQueue::PtrQueue(PtrQueue &&o)
{
	std::swap(_data, o._data);
	_chunkSize = std::move(o._chunkSize);
	_cursor = std::move(o._cursor);
	_to = std::move(o._to);
	std::swap(_size, o._size);
	o.clear();
}

PtrQueue::~PtrQueue()
{
	if (_data != nullptr)
		free(_data);
}

void PtrQueue::pop()
{
	if (empty())
		return;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	_cursor += s + soi;
	tmp += soi;
	auto r = reinterpret_cast<MessageBase*>(tmp);
	r->~MessageBase();
}

MessageBase *PtrQueue::front()
{
	if (empty())
		return nullptr;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	tmp += soi;
	return ((MessageBase*)(tmp));
}

void PtrQueue::clear()
{
	_cursor = _to = 0;
}

void PtrQueue::release()
{
	clear();
	if (_data != nullptr)
		free(_data);
	_data = nullptr;
}

bool PtrQueue::empty()
{
	if (_cursor >= _to)
	{
		_cursor = 0;
		_to = 0;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

Dispatcher Queue::getDispatcher()
{
	return Dispatcher(this);
}


Queue::Queue()
	: _publisherThreadId(0)
	, _millisecondToWait(1)
{
}

void Queue::launch()
{
	_writeCondition.notify_one();
}

void Queue::getReadableQueue(PtrQueue& q)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_readCondition.wait_for(lock, std::chrono::milliseconds(1), [this](){ return !_copy.empty() || !_priorityCopy.empty(); }))
		return;
	if (!_priorityCopy.empty())
	{
		q = std::move(_priorityCopy);
		_priorityCopy.clear();
	}
	else
	{
		q = std::move(_copy);
		_copy.clear();
	}
	lock.unlock();
	_writeCondition.notify_one();
}

void Queue::releaseReadability()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_writeCondition.wait(lock, [this]()
	{
		return (_copy.empty() && _priorityCopy.empty());
	});
	if (!_priority.empty() && _priorityCopy.empty())
	{
		_priorityCopy = std::move(_priority);
		_priority.clear();
		lock.unlock();
	}
	else if (!_queue.empty() && _copy.empty())
	{
		_copy = std::move(_queue);
		_queue.clear();
		lock.unlock();
	}
	else
	{
		lock.unlock();
	}
	_readCondition.notify_one();
}

void Queue::setWaitingTime(std::size_t milliseconds)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_millisecondToWait = milliseconds;
}

std::size_t Queue::getWaitingTime()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _millisecondToWait;
}
