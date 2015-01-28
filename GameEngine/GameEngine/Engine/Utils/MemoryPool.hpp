#pragma once

#include <vector>
#include <queue>
#include <stdint.h>
#include <assert.h>

template<class T>
class MemoryPool
{
public:
	MemoryPool() :
		_currentIdx(0)
	{
		_currentIdx = 0;
	}

	MemoryPool(MemoryPool &&move) :
		_pool(std::move(move._pool)),
		_currentIdx(move._currentIdx),
		_freeIdx(std::move(move._freeIdx))
	{

	}

	MemoryPool(uint32_t maxSize) :
		_currentIdx(0)
	{
		_pool.reserve(maxSize);
	}

	~MemoryPool() { }

	template <class... param_t>
	uint32_t alloc(param_t... param)
	{
		if (_freeIdx.empty() == false)
		{
			uint32_t ret = _freeIdx.front();
			_freeIdx.pop();
			new (&_pool[ret]) T(param...);
			return (ret);
		}
		_pool.emplace_back(param...);
		return (_currentIdx++);
	}

	uint32_t prepareAlloc()
	{
		if (_freeIdx.empty() == false)
		{
			uint32_t ret = _freeIdx.front();
			_freeIdx.pop();
			return (ret);
		}
		return (_currentIdx++);
	}

	template <class... param_t>
	void allocPreparated(uint32_t idx, param_t... param)
	{
		assert(idx <= _pool.size());
		if (idx == _pool.size())
			_pool.emplace_back(param...);
		else
			new (&_pool[idx]) T(param...);
	}

	void dealloc(uint32_t idx)
	{
		_pool[idx].~T();
		_freeIdx.push(idx);
	}

	void prepareDealloc(uint32_t idx)
	{
		_freeIdx.push(idx);
	}

	void deallocPreparated(uint32_t idx)
	{
		_pool[idx].~T();
	}

	T &get(uint32_t idx)
	{
		return (_pool[idx]);
	}

	T const &get(uint32_t idx) const
	{
		return (_pool[idx]);
	}

private:
	std::vector<T>			_pool;
	std::queue<uint32_t>	_freeIdx;
	uint32_t				_currentIdx;
};