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

	MemoryPool(uint32_t maxSize) :
		_currentIdx(0)
	{
		_pool.reserve(maxSize);
	}

	~MemoryPool() { }

	uint32_t alloc()
	{
		if (_freeIdx.empty() == false)
		{
			uint32_t ret = _freeIdx.front();
			_freeIdx.pop();
			new (&_pool[ret]) T();
			return (ret);
		}
		_pool.emplace_back();
		return (_currentIdx++);
	}

	void dealloc(uint32_t idx)
	{
		_pool[idx].~T();
		_freeIdx.push(idx);
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