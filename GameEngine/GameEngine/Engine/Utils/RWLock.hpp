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
		RWLockGuard(RWLock &lock, bool exclusive);
		~RWLockGuard();

	private:
		RWLock &_lock;
		bool _exclusive;
	};
}