#include "SpinLock.hpp"

namespace AGE
{
	void SpinLock::lock(void)
	{
		while (atomicLock.exchange(true));
	}

	void SpinLock::lock(void) volatile
	{
		while (atomicLock.exchange(true));
	}

	void SpinLock::unlock(void)
	{
		atomicLock = false;
	}

	void SpinLock::unlock(void) volatile
	{
		atomicLock = false;
	}

	bool SpinLock::isLocked(void) const
	{
		return atomicLock;
	}
}
