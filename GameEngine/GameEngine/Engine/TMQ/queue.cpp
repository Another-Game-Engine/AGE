#include "queue.hpp"
#include "dispatcher.hpp"

using namespace TMQ;

PtrQueue::Chunk::Chunk(std::size_t chunkSize)
	: _data(nullptr)
	, _cursor(0)
	, _size(chunkSize)
	, _to(0)
{
	assert(chunkSize != 0);
	_data = (char*)malloc(chunkSize);
}

PtrQueue::Chunk& PtrQueue::Chunk::operator=(Chunk &&o)
{
	std::swap(_data, o._data);
	std::swap(_cursor, o._cursor);
	std::swap(_to, o._to);
	std::swap(_size, o._size);
	return *this;
}

PtrQueue::Chunk::Chunk(Chunk &&o)
{
	std::swap(_data, o._data);
	std::swap(_cursor, o._cursor);
	std::swap(_to, o._to);
	std::swap(_size, o._size);
}

PtrQueue::Chunk::~Chunk()
{
	if (_data)
		delete _data;
}

void PtrQueue::Chunk::pop()
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


MessageBase *PtrQueue::Chunk::front()
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

std::size_t PtrQueue::Chunk::getFrontSize()
{
	if (empty())
		return 0;

	char *tmp = _data;
	std::size_t soi = sizeof(std::size_t);

	tmp += _cursor;
	std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
	return s;
}

void PtrQueue::Chunk::clear()
{
	_cursor = _to = 0;
}

void PtrQueue::Chunk::eraseAll()
{
	while (!empty())
	{
		pop();
	}
	clear();
}

bool PtrQueue::Chunk::empty()
{
	if (_cursor >= _to)
	{
		_cursor = 0;
		_to = 0;
		return true;
	}
	return false;
}


PtrQueue::PtrQueue(std::size_t chunkSize)
	: _chunkSize(chunkSize)
{
	_list.push_back(new Chunk(chunkSize));
	_listWriter = std::begin(_list);
	_listReader = std::begin(_list);
}


PtrQueue& PtrQueue::operator=(PtrQueue &&o)
{
	std::swap(_chunkSize, o._chunkSize);
	std::swap(_list, o._list);
	std::swap(_listReader, o._listReader);
	std::swap(_listWriter, o._listWriter);
	_listWriter = std::begin(_list);
	_listReader = std::begin(_list);
	return *this;
}

PtrQueue::PtrQueue(PtrQueue &&o)
{
	std::swap(_chunkSize, o._chunkSize);
	std::swap(_list, o._list);
	std::swap(_listReader, o._listReader);
	std::swap(_listWriter, o._listWriter);
	_listWriter = std::begin(_list);
	_listReader = std::begin(_list);
}

PtrQueue::~PtrQueue()
{
	for (auto &e : _list)
		delete e;
	_list.clear();
}

void PtrQueue::pop()
{
	if (empty())
		return;
	(*_listReader)->pop();
	if ((*_listReader)->empty())
		++_listReader;
	_listWriter = std::begin(_list);
}

MessageBase *PtrQueue::front()
{
	if (empty())
		return nullptr;
	_listWriter = std::begin(_list);
	return  (*_listReader)->front();
}

std::size_t PtrQueue::getFrontSize()
{
	if (empty())
		return 0;

	return  (*_listReader)->getFrontSize();
}


void PtrQueue::clear()
{
	_listWriter = std::begin(_list);
	for (auto &e : _list)
		e->clear();
}

void PtrQueue::eraseAll()
{
	_listWriter = std::begin(_list);
	for (auto &e : _list)
		e->eraseAll();
}

bool PtrQueue::empty()
{
	if (_listReader == std::end(_list))
	{
		return true;
	}
	if (!(*_listReader)->empty())
		return false;
	if (++_listReader != std::end(_list))
		return (*_listReader)->empty();
	return true;
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
		std::unique_lock<std::mutex> lock(_mutex);
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
}

bool HybridQueue::releaseTaskReadability()
{
	_releasable = true;
	_readCondition.notify_one();
	return true;
}

bool HybridQueue::releaseCommandReadability(WaitType waitType)
{
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