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

	MemoryPool(std::size_t maxSize) :
		_currentIdx(0)
	{
		_pool.reserve(maxSize);
	}

	~MemoryPool() { }

	template <class... param_t>
	std::size_t alloc(param_t... param)
	{
		if (_freeIdx.empty() == false)
		{
			std::size_t ret = _freeIdx.front();
			_freeIdx.pop();
			new (&_pool[ret]) T(param...);
			return (ret);
		}
		_pool.emplace_back(param...);
		return (_currentIdx++);
	}

	std::size_t prepareAlloc()
	{
		if (_freeIdx.empty() == false)
		{
			std::size_t ret = _freeIdx.front();
			_freeIdx.pop();
			return (ret);
		}
		auto res = _currentIdx;
		++_currentIdx;
		return res;
	}

	template <class... param_t>
	void allocPreparated(std::size_t idx, param_t... param)
	{
		if (idx == _pool.size())
			_pool.emplace_back(param...);
		else
		{
			if (idx >= _pool.size())
			{
				_pool.resize(idx + 1);
			}
			new (&_pool[idx]) T(param...);
		}
	}

	void dealloc(std::size_t idx)
	{
		_pool[idx].~T();
		_freeIdx.push(idx);
	}

	void prepareDealloc(std::size_t idx)
	{
		_freeIdx.push(idx);
	}

	void deallocPreparated(std::size_t idx)
	{
		_pool[idx].~T();
	}

	T &get(std::size_t idx)
	{
		return (_pool[idx]);
	}

	T const &get(std::size_t idx) const
	{
		return (_pool[idx]);
	}

private:
	std::vector<T>	_pool;
	std::queue<std::size_t>	_freeIdx;
	std::size_t				_currentIdx;
};