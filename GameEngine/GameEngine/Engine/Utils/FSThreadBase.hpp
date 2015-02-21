#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

#include "SpinLock.hpp"

namespace AGE
{
	class FSThreadBase
	{
	private:
		std::thread thread;
		std::atomic<bool> running = false;
		std::atomic<bool> waiting = false;
		std::mutex mutex;
		std::condition_variable condition;
		SpinLock locker;

	protected:
		virtual void process(void) = 0;

		void pause(void);

	public:
		FSThreadBase(void) = default;
		FSThreadBase(const FSThreadBase &other) = delete;
		FSThreadBase &operator=(const FSThreadBase &other) = delete;
		virtual ~FSThreadBase(void);

		void run(void);
		void resume(void);
		void stop(void);

		bool isRunning(void) const;
		bool isWaiting(void) const;

		void lock(void);
		void unlock(void);
		bool isLocked(void) const;

		static void Handler(FSThreadBase *thread);
	};
}
