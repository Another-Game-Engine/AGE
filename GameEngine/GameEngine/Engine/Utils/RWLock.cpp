#include "RWLock.hpp"
#include "Debug.hpp"

#include <thread>

namespace AGE
{
	RWLock::RWLock()
	{
		_sharedLock.store(0, std::memory_order_relaxed);
		_uniqueLock.store(false, std::memory_order_relaxed);
	}

	void RWLock::ReadLock()
	{
		while (_uniqueLock.exchange(true, std::memory_order_relaxed))
		{
			std::this_thread::yield();
		}
		_sharedLock.fetch_add(1, std::memory_order_relaxed);
		_uniqueLock.store(false, std::memory_order_relaxed);
	}

	void RWLock::ReadUnlock()
	{
		_sharedLock.fetch_sub(1, std::memory_order_relaxed);
	}

	void RWLock::WriteLock()
	{
		while (_uniqueLock.exchange(true, std::memory_order_relaxed))
		{
			std::this_thread::yield();
		}
		while (_sharedLock.load(std::memory_order_relaxed) > 0)
		{
			std::this_thread::yield();
		}
	}

	void RWLock::WriteUnlock()
	{
		_uniqueLock.store(false, std::memory_order_relaxed);
	}

	// RWLockGuard

	RWLockGuard::RWLockGuard(RWLock &lock, bool exclusive)
		: _lock(lock)
		, _exclusive(exclusive)
	{
		if (_exclusive)
		{
			_lock.WriteLock();
		}
		else
		{
			_lock.ReadLock();
		}
	}

	RWLockGuard::~RWLockGuard()
	{
		if (_exclusive)
		{
			_lock.WriteUnlock();
		}
		else
		{
			_lock.ReadUnlock();
		}
	}


	// End RWLockGuard
}