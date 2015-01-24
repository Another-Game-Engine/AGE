#pragma once

#include <vector>
#include <queue>
#include <stdint.h>
#include <assert.h>

template<class T>
class MemoryPool
{
public:
	MemoryPool(uint32_t maxSize)
	{
		_currentIdx = 0;
		_pool.reserve(maxSize);
	}

	MemoryPool(MemoryPool &&move) :
		_pool(std::move(move._pool)),
		_currentIdx(move._currentIdx),
		_freeIdx(std::move(move._freeIdx))
	{

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
	std::vector<T>	_pool;
	std::queue<uint32_t>	_freeIdx;
	uint32_t				_currentIdx;
};