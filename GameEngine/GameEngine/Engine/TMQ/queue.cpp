#include "queue.hpp"
#include "dispatcher.hpp"

using namespace TMQ;

PtrQueue HybridQueue::_sharedQueue;
std::mutex HybridQueue::_mutex;
std::condition_variable HybridQueue::_condition;

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
{
}

void HybridQueue::getTask(MessageBase *& task)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [this](){ return (_individualQueue.empty() == false || _sharedQueue.empty() == false); });

	task = _individualQueue.front();
	if (task)
	{
		return;
	}

	task = _sharedQueue.front();
}


