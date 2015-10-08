#pragma once

#include "message.hpp"

#include <concurrentqueue/blockingconcurrentqueue.h>

#include <Utils/Profiler.hpp>
#include "Utils/Debug.hpp"

namespace TMQ
{

	typedef moodycamel::ConcurrentQueue<MessageBase*> MessageQueue;
	typedef moodycamel::details::mpmc_sema::LightweightSemaphore  LWSemapore;

	class TaskManager
	{
	private:
		struct RenderThreadQueue
		{
			static MessageQueue individualQueue;
			static LWSemapore   individualSemaphore;
		};

		struct MainThreadQueue
		{
			static MessageQueue individualQueue;
		};

		struct TaskQueue
		{
			static MessageQueue queue;
			static LWSemapore   semaphore;
		};
	public:
		static bool MainThreadGetTask(MessageBase *& task)
		{
			task = nullptr;
			if (!MainThreadQueue::individualQueue.try_dequeue(task))
			{
				return TaskQueue::queue.try_dequeue(task);
			}
			return true;
		}

		static bool RenderThreadGetTask(MessageBase *& task)
		{
			task = nullptr;
			RenderThreadQueue::individualSemaphore.wait();

			if (!RenderThreadQueue::individualQueue.try_dequeue(task))
			{
				return TaskQueue::queue.try_dequeue(task);
			}
			return true;
		}

		static bool TaskThreadGetTask(MessageBase *& task)
		{
			task = nullptr;
			TaskQueue::semaphore.wait();

			return TaskQueue::queue.try_dequeue(task);
		}


		template <typename T>
		static void pushSharedTask(const T& e)
		{
			SCOPE_profile_cpu_function("TMQ");
			TaskQueue::queue.enqueue(new Message<T>(e));
			TaskQueue::semaphore.signal();
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename ...Args>
		static void emplaceSharedTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			TaskQueue::queue.enqueue(new Message<T>(args...));
			TaskQueue::semaphore.signal();
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename F>
		static std::future<F> pushSharedFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = new Message<T>(e);
			f = tmp->getData().getFuture();
			TaskQueue::queue.enqueue(tmp);
			TaskQueue::semaphore.signal();
			RenderThreadQueue::individualSemaphore.signal();
			return f;
		}

		template <typename T, typename F, typename ...Args>
		static std::future<F> emplaceSharedFutureTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future< F > f;
			auto tmp = new Message<T>(args...);
			f = tmp->getData().getFuture();
			TaskQueue::queue.enqueue(tmp);
			TaskQueue::semaphore.signal();
			RenderThreadQueue::individualSemaphore.signal();
			return f;
		}

		////////////////
		template <typename T>
		static void pushRenderTask(const T& e)
		{
			SCOPE_profile_cpu_function("TMQ");
			RenderThreadQueue::individualQueue.enqueue(new Message<T>(e));
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename ...Args>
		static void emplaceRenderTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			RenderThreadQueue::individualQueue.enqueue(new Message<T>(args...));
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename F>
		static std::future<F> pushRenderFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = new Message<T>(e);
			f = tmp->getData().getFuture();
			RenderThreadQueue::individualQueue.enqueue(tmp);
			RenderThreadQueue::individualSemaphore.signal();
			return f;
		}

		template <typename T, typename F, typename ...Args>
		static std::future<F> emplaceRenderFutureTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future< F > f;
			auto tmp = new Message<T>(args...);
			f = tmp->getData().getFuture();
			RenderThreadQueue::individualQueue.enqueue(tmp);
			RenderThreadQueue::individualSemaphore.signal();
			return f;
		}


		////////////////
		template <typename T>
		static void pushMainTask(const T& e)
		{
			SCOPE_profile_cpu_function("TMQ");
			MainThreadQueue::individualQueue.enqueue(new Message<T>(e));
		}

		template <typename T, typename ...Args>
		static void emplaceMainTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			MainThreadQueue::individualQueue.enqueue(new Message<T>(args...));
		}

		template <typename T, typename F>
		static std::future<F> pushMainFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = new Message<T>(e);
			f = tmp->getData().getFuture();
			MainThreadQueue::individualQueue.enqueue(tmp);
			return f;
		}

		template <typename T, typename F, typename ...Args>
		static std::future<F> emplaceMainFutureTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future< F > f;
			auto tmp = new Message<T>(args...);
			f = tmp->getData().getFuture();
			MainThreadQueue::individualQueue.enqueue(tmp);
			return f;
		}
	};
}