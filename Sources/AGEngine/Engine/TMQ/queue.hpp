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
		struct CircularBuffer
		{
			static const std::size_t BufferSize = 10000000; // 10 mega that's a lot too much but because
			// we don't have any security to be sure that we didn't write over non deleted data it's better to be safe
			char *buffer;
			std::atomic_size_t index;

			CircularBuffer()
			{
				index = 0;
				buffer = (char*)malloc(BufferSize);
				AGE_ASSERT(buffer != nullptr);
			}

			void *allocMem(std::size_t size)
			{
				while (true)
				{
					std::size_t i = index.load();
					std::size_t newIndex = i + size;

					if (newIndex >= BufferSize)
					{
						if (index.compare_exchange_weak(i, size))
						{
							return (void*)&buffer[0];
						}
						continue;
					}
					if (index.compare_exchange_weak(i, newIndex))
					{
						return (void*)&buffer[i];
					}
				}
			}

			template <typename T, typename ...Args>
			T *allocate(Args... args)
			{
				std::size_t size = sizeof(T);
				while (true)
				{
					std::size_t i = index.load();
					std::size_t newIndex = i + size;

					if (newIndex >= BufferSize)
					{
						if (index.compare_exchange_weak(i, size))
						{
							return new(&buffer[0])T(args...);
						}
						continue;
					}
					if (index.compare_exchange_weak(i, newIndex))
					{
						return new(&buffer[i])T(args...);
					}
				}
			}

			template <typename T>
			T *allocateMultiple(std::size_t number)
			{
				std::size_t size = sizeof(T) * number;
				while (true)
				{
					std::size_t i = index.load();
					std::size_t newIndex = i + size;

					if (newIndex >= BufferSize)
					{
						if (index.compare_exchange_weak(i, size))
						{
							return (T*)(&buffer[0]);
						}
						continue;
					}
					if (index.compare_exchange_weak(i, newIndex))
					{
						return (T*)(&buffer[i]);
					}
				}
			}
		};
		struct RenderThreadQueue
		{
			static MessageQueue individualQueue;
			static LWSemapore   individualSemaphore;
			static CircularBuffer circularBuffer;
		};

		struct MainThreadQueue
		{
			static MessageQueue individualQueue;
			static CircularBuffer circularBuffer;
		};

		struct TaskQueue
		{
			static MessageQueue queue;
			static LWSemapore   semaphore;
			static CircularBuffer circularBuffer;
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
			TaskQueue::queue.enqueue(TaskQueue::circularBuffer.allocate<Message<T>>(e));
			TaskQueue::semaphore.signal();
			RenderThreadQueue::individualSemaphore.signal();
		}

		// They are allocated but NOT CONSTRUCTED !!!
		template <typename T>
		static Message<T> *allocSharedTasks(std::size_t number)
		{
			return TaskQueue::circularBuffer.allocateMultiple<Message<T>>(number);
		}

		template <typename T>
		static void pushAllocatedSharedTasks(T *tasks, std::size_t number)
		{
			for (std::size_t i = 0; i < number; ++i)
				TaskQueue::queue.enqueue(&tasks[i]);
			TaskQueue::semaphore.signal(number);
			RenderThreadQueue::individualSemaphore.signal(number);
		}

		template <typename T, typename ...Args>
		static void emplaceSharedTask(Args... args)
		{
			TaskQueue::queue.enqueue(TaskQueue::circularBuffer.allocate<Message<T>>(args...));
			TaskQueue::semaphore.signal();
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename F>
		static std::future<F> pushSharedFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = TaskQueue::circularBuffer.allocate<Message<T>>(e);
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
			auto tmp = TaskQueue::circularBuffer.allocate<Message<T>>(args...);
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
			RenderThreadQueue::individualQueue.enqueue(RenderThreadQueue::circularBuffer.allocate<Message<T>>(e));
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename ...Args>
		static void emplaceRenderTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			RenderThreadQueue::individualQueue.enqueue(RenderThreadQueue::circularBuffer.allocate<Message<T>>(args...));
			RenderThreadQueue::individualSemaphore.signal();
		}

		template <typename T, typename F>
		static std::future<F> pushRenderFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = RenderThreadQueue::circularBuffer.allocate<Message<T>>(e);
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
			auto tmp = RenderThreadQueue::circularBuffer.allocate<Message<T>>(args...);
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
			MainThreadQueue::individualQueue.enqueue(MainThreadQueue::circularBuffer.allocate<Message<T>>(e));
		}

		template <typename T, typename ...Args>
		static void emplaceMainTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			MainThreadQueue::individualQueue.enqueue(MainThreadQueue::circularBuffer.allocate<Message<T>>(args...));
		}

		template <typename T, typename F>
		static std::future<F> pushMainFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = MainThreadQueue::circularBuffer.allocate<Message<T>>(e);
			f = tmp->getData().getFuture();
			MainThreadQueue::individualQueue.enqueue(tmp);
			return f;
		}

		template <typename T, typename F, typename ...Args>
		static std::future<F> emplaceMainFutureTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future< F > f;
			auto tmp = MainThreadQueue::circularBuffer.allocate<Message<T>>(args...);
			f = tmp->getData().getFuture();
			MainThreadQueue::individualQueue.enqueue(tmp);
			return f;
		}
	};
}