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
	std::swap(_chunkSize, o._chunkSize);
	std::swap(_cursor, o._cursor);
	std::swap(_to, o._to);
	std::swap(_size, o._size);
//	o.clear();
	return *this;
}

PtrQueue::PtrQueue(PtrQueue &&o)
{
	std::swap(_data, o._data);
	std::swap(_chunkSize, o._chunkSize);
	std::swap(_cursor, o._cursor);
	std::swap(_to, o._to);
	std::swap(_size, o._size);
//	o.clear();
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

std::size_t PtrQueue::getFrontSize()
{
	if (empty())
		return 0;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	return s;
}


void PtrQueue::clear()
{
	_cursor = _to = 0;
}

void PtrQueue::eraseAll()
{
	while (!empty())
	{
		pop();
	}
	clear();
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


////////////////////
/// HYBRID

HybridQueue::HybridQueue()
	: _millisecondToWait(1)
	, _reservedPublisher(-1)
{
	_releasable = true;
}

void HybridQueue::launch()
{
	_writeCondition.notify_one();
}

bool HybridQueue::getTaskQueue(TMQ::PtrQueue &q, WaitType waitType)
{
   	if (waitType == WaitType::NoWait)
	{
		if (!_releasable)
			return false;
		std::unique_lock<std::mutex> lock(_mutex);
		q = std::move(_taskQueue);
		_taskQueue.clear();
		_releasable = false;
		lock.unlock();
		return true;
	}
	else if (waitType == WaitType::Block)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		_readCondition.wait(lock, [this]()
		{
			return (_releasable || !_taskQueue.empty());
		});
		if (_taskQueue.empty() && !_releasable)
			return false;
		q = std::move(_taskQueue);
		_taskQueue.clear();
		_releasable = false;
		lock.unlock();
		return true;
	}
	else if (waitType == WaitType::Wait)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_readCondition.wait_for(lock, std::chrono::microseconds(1), [this]()
		{
			return (!(_releasable || _taskQueue.empty()));
		}))
		{
			return false;
		}
		q = std::move(_taskQueue);
		_taskQueue.clear();
		_releasable = false;
		lock.unlock();
		return true;
	}
	return true;
}

void HybridQueue::getTaskAndCommandQueue(
	TMQ::PtrQueue &taskQueue,
	bool &taskQueueSuccess,
	TMQ::PtrQueue &commandQueue,
	bool &commandQueueSuccess,
	WaitType waitType)
{
	taskQueueSuccess = commandQueueSuccess = false;
   	if (waitType == WaitType::NoWait)
	{
		if (!_releasable)
		{
			taskQueueSuccess = commandQueueSuccess = false;
			return;
		}
		std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
		if (!lock.owns_lock())
			return;
		taskQueue = std::move(_taskQueue);
		_taskQueue.clear();
		commandQueue = std::move(_commandQueueCopy);
		_commandQueueCopy.clear();
		_releasable = false;
		lock.unlock();
		taskQueueSuccess = !taskQueue.empty();
		commandQueueSuccess = !commandQueue.empty();
		if (commandQueueSuccess)
			_writeCondition.notify_one();
		return;
	}
	else if (waitType == WaitType::Block)
	{
		std::unique_lock<std::mutex> lock(_mutex); //ici fuck !!!
		_readCondition.wait(lock, [this]()
		{
			return (_releasable || !_taskQueue.empty() || !_commandQueueCopy.empty());
		});
		taskQueue = std::move(_taskQueue);
		_taskQueue.clear();
		commandQueue = std::move(_commandQueueCopy);
		_commandQueueCopy.clear();
		_releasable = false;
		lock.unlock();
		taskQueueSuccess = !taskQueue.empty();
		commandQueueSuccess = !commandQueue.empty();
		if (commandQueueSuccess)
			_writeCondition.notify_one();
		return;
	}
	else if (waitType == WaitType::Wait)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_readCondition.wait_for(lock, std::chrono::microseconds(1), [this]()
		{
			return (!(_releasable || !_taskQueue.empty() || !_commandQueueCopy.empty()));
		}))
		{
			commandQueueSuccess = taskQueueSuccess = false;
			return;
		}
		taskQueue = std::move(_taskQueue);
		commandQueue = std::move(_commandQueueCopy);
		_taskQueue.clear();
		_commandQueueCopy.clear();
		_releasable = false;
		lock.unlock();
		taskQueueSuccess = !taskQueue.empty();
		commandQueueSuccess = !commandQueue.empty();
		if (commandQueueSuccess)
			_writeCondition.notify_one();
	}
}

bool HybridQueue::releaseTaskReadability()
{
	_releasable = true;
	_readCondition.notify_one();
	return true;
}

bool HybridQueue::releaseCommandReadability(WaitType waitType)
{
	try{
		if (waitType == WaitType::NoWait)
		{
			std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
			if (!lock.owns_lock())
				return false;
			if (!_commandQueueCopy.empty())
				return false;
			_commandQueueCopy = std::move(_commandQueue);
			_commandQueue.clear();
			_releasable = true;
			lock.unlock();
			_readCondition.notify_one();
			return true;
		}
		else if (waitType == WaitType::Block)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_writeCondition.wait(lock, [this]()
			{
				return (_commandQueueCopy.empty());
			});
			if (!_commandQueueCopy.empty())
				return false;
			_commandQueueCopy = std::move(_commandQueue);
			_commandQueue.clear();
			_releasable = true;
			lock.unlock();
			_readCondition.notify_one();
			return true;
		}
		else if (waitType == WaitType::Wait)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (!_writeCondition.wait_for(lock, std::chrono::microseconds(1), [this]()
			{
				return (_commandQueueCopy.empty());
			}))
			{
				return false;
			}
			if (!lock.owns_lock())
				assert(false);
			_commandQueueCopy = std::move(_commandQueue);
			_commandQueue.clear();
			_releasable = true;
			lock.unlock();
			_readCondition.notify_one();
			return true;
		}
	}
	catch (std::exception e)
	{
		auto t = e.what();
		auto tt = t;
	}
	return true;
}

void HybridQueue::setWaitingTime(std::size_t milliseconds)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_millisecondToWait = milliseconds;
}

std::size_t HybridQueue::getWaitingTime()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _millisecondToWait;
}

void HybridQueue::clear()
{
	std::unique_lock<std::mutex> lock(_mutex);
	//while (!lock.owns_lock())
	//{
	//	lock.lock();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	//}
	_commandQueue.eraseAll();
}