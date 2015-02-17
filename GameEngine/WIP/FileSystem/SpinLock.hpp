#pragma once

#include <atomic>

namespace Age
{
	namespace Engine
	{
		class SpinLock final
		{
		public:
			SpinLock(void) = default;
			SpinLock(const SpinLock &) = delete;
			SpinLock(SpinLock &&) = delete;
			SpinLock &operator=(const SpinLock &) = delete;
			SpinLock &operator=(SpinLock &&) = delete;
			~SpinLock(void) = default;

			void lock(void);
			void lock(void) volatile;
			void unlock(void);
			void unlock(void) volatile;
			bool isLocked(void) const;

		private:
			std::atomic<bool> atomicLock = false;
		};
	}
}