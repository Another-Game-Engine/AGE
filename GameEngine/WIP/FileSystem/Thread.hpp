#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

#include "SpinLock.hpp"

namespace Age
{
	namespace Engine
	{
		class Thread
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
			Thread(void) = default;
			Thread(const Thread &other) = delete;
			Thread &operator=(const Thread &other) = delete;
			virtual ~Thread(void);

			void run(void);
			void resume(void);
			void stop(void);

			bool isRunning(void) const;
			bool isWaiting(void) const;

			void lock(void);
			void unlock(void);
			bool isLocked(void) const;

			static void Handler(Thread *thread);
		};
	}
}