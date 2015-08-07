#pragma once

#include <atomic>

namespace AGE
{
	class RWLock
	{
	public:
		RWLock();
		void ReadLock();
		void ReadUnlock();
		void WriteLock();
		void WriteUnlock();
	private:
		std::atomic_uint16_t _sharedLock;
		std::atomic<bool> _uniqueLock;
	};

	struct RWLockGuard
	{
		RWLockGuard(RWLock &lock, bool exclusive)
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

		~RWLockGuard()
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
	private:
		RWLock &_lock;
		bool _exclusive;
	};
}