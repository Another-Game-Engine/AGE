#pragma once

#include <vector>
#include <queue>
#include <stdint.h>
#include <assert.h>

template<class T, typename IndexType = std::size_t>
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

	MemoryPool(IndexType maxSize) :
		_currentIdx(0)
	{
		_pool.reserve(maxSize);
	}

	~MemoryPool() { }

	template <class... param_t>
	IndexType alloc(param_t... param)
	{
		if (_freeIdx.empty() == false)
		{
			IndexType ret = _freeIdx.front();
			_freeIdx.pop();
			new (&_pool[ret]) T(param...);
			return (ret);
		}
		_pool.emplace_back(param...);
		return (_currentIdx++);
	}

	IndexType prepareAlloc()
	{
		if (_freeIdx.empty() == false)
		{
			IndexType ret = _freeIdx.front();
			_freeIdx.pop();
			return (ret);
		}
		auto res = _currentIdx;
		++_currentIdx;
		return res;
	}

	template <class... param_t>
	void allocPreparated(IndexType idx, param_t... param)
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

	void dealloc(IndexType idx)
	{
		_pool[idx].~T();
		_freeIdx.push(idx);
	}

	void prepareDealloc(IndexType idx)
	{
		_freeIdx.push(idx);
	}

	void deallocPreparated(IndexType idx)
	{
		_pool[idx].~T();
	}

	T &get(IndexType idx)
	{
		return (_pool[idx]);
	}

	T const &get(IndexType idx) const
	{
		return (_pool[idx]);
	}

private:
	std::vector<T>	_pool;
	std::queue<IndexType>	_freeIdx;
	IndexType				_currentIdx;
};