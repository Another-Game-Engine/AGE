#pragma once

#include "message.hpp"

#include <cstdlib>
#include <new>
#include <utility>
#include <cinttypes>
#include <condition_variable>
#include <mutex>

namespace TMQ
{
	class Queue;
	class Dispatcher;
	class Messsage;

	class PtrQueue
	{
	public:
		PtrQueue(const PtrQueue &o) = delete;
		PtrQueue& operator=(const PtrQueue &o) = delete;
		PtrQueue(std::size_t chunkSize = 1024);
		PtrQueue& operator=(PtrQueue &&o);
		PtrQueue(PtrQueue &&o);
		~PtrQueue();
		void pop();
		MessageBase *front();
		void clear();
		void release();
		bool empty();
	private:
		template <typename T>
		T* push(T&& e)
		{
			std::size_t s = sizeof(Message<T>);
			std::size_t sizeOfInt = sizeof(std::size_t);

			if (_data == nullptr
				|| _size - _to < s + sizeOfInt)
			{
				allocate<T>();
			}

			char *tmp = _data;
			tmp += _to;
			memcpy(tmp, &s, sizeOfInt);
			tmp += sizeOfInt;
			Message<T>* res = new(tmp)Message<T>(e);
			_to += sizeOfInt + s;
			return &res->_data;
		}

		template <typename T, typename ...Args>
		T* emplace(Args ...args)
		{
			std::size_t s = sizeof(Message<T>);
			std::size_t sizeOfInt = sizeof(std::size_t);

			if (_data == nullptr
				|| _size - _to < s + sizeOfInt)
			{
				allocate<T>();
			}

			char *tmp = _data;
			tmp += _to;
			memcpy(tmp, &s, sizeOfInt);
			tmp += sizeOfInt;
			Message<T>* res = new(tmp)Message<T>(args...);
			_to += sizeOfInt + s;
			return &res->_data;
		}

		template <typename T>
		void allocate()
		{
			std::size_t sizeOfType = sizeof(Message<T>);

			while (_size - _to <= sizeOfType + sizeof(std::size_t)
				|| _size <= _to)
			{
				_size += _chunkSize;
				_data = (char*)(realloc(_data, _size));
			}
		}

		std::size_t _chunkSize;
		char *_data;
		std::size_t _cursor;
		std::size_t _size;
		std::size_t _to;

		friend class Queue;
	};

	class Queue
	{
		PtrQueue _queue;
		PtrQueue _copy;
		PtrQueue _priority;
		PtrQueue _priorityCopy;
		std::mutex _mutex;
		std::condition_variable _readCondition;
		std::condition_variable _writeCondition;
		std::size_t _publisherThreadId;
		std::once_flag _onceFlag;
		std::size_t _millisecondToWait;
	public:
		Queue();
		void launch();

		Queue(const Queue&) = delete;
		Queue &operator=(const Queue&) = delete;
		Queue(Queue &&) = delete;
		Queue operator=(Queue &&) = delete;

		void getReadableQueue(PtrQueue& q);
		Dispatcher getDispatcher();
		void releaseReadability();
		void setWaitingTime(std::size_t milliseconds);
		std::size_t getWaitingTime();

		//////
		////// Internal standard queue access

		//Do not lock mutex
		//Use it only if used in the same thread, or use safePush
		template <typename T>
		T* push(const T& e)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			return _queue.push(e);
		}

		//Do not lock mutex
		//Use it only if used in the same thread, or use safeEmplace
		template <typename T, typename ...Args>
		T* emplace(Args ...args)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			return _queue.emplace<T>(args...);
		}

		//Lock mutex
		//Can be called simutanously in different threads
		template <typename T>
		void safePush(const T& e)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push(e);
		}

		//Lock mutex
		//Can be called simutanously in different threads
		template <typename T, typename ...Args>
		void safeEmplace(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.emplace<T>(args...);
		}

		//Lock mutex or not
		//Depend of the usage you made before
		template <typename T>
		void autoPush(const T& e)
		{
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push(e);
			}
			else
			{
				_queue.push(e);
			}
		}

		//Lock mutex or not
		//Depend of the usage you made before
		template <typename T, typename ...Args>
		void autoEmplace(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.emplace<T>(args...);
			}
			else
			{
				_queue.emplace<T>(args...);
			}
		}

		//Push in queue and return future
		//Message data have to heritate from FutureData
		template <typename T, typename F>
		std::future<F> pushFuture(const T &e)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			return _queue.push(e)->getFuture();
		}

		//Emplace in queue and return future
		//Message data have to heritate from FutureData
		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFuture(Args ...args)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			return _queue.emplace<T>(args...)->getFuture();
		}

		//Push in queue and return future
		//Message data have to heritate from FutureData
		template <typename T, typename F>
		std::future<F> safePushFuture(const T &e)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			std::lock_guard<std::mutex> lock(_mutex);			
			return _queue.push(e)->getFuture();
		}

		//Emplace in queue and return future
		//Message data have to heritate from FutureData
		template <typename T, typename F, typename ...Args>
		std::future<F> safeEmplaceFuture(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.emplace<T>(args...)->getFuture();
		}

		//Lock mutex or not
		//Depend of the usage you made before
		template <typename T, typename F>
		std::future<F> autoPushFuture(const T &e)
		{
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				return _queue.push(e)->getFuture();
			}
			else
			{
				return _queue.push(e)->getFuture();
			}
		}

		//Lock mutex or not
		//Depend of the usage you made before
		template <typename T, typename F, typename ...Args>
		std::future<F> autoEmplaceFuture(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				return _queue.emplace<T>(args...)->getFuture();
			}
			else
			{
				return _queue.emplace<T>(args...)->getFuture();
			}
		}

		//////
		////// Internal priority queue access

		template <typename T>
		void priorityPush(const T& e)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			_queue.push(e);
			releaseReadability();
		}

		template <typename T, typename ...Args>
		void priorityEmplace(Args ...args)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			_queue.emplace<T>(args...);
			releaseReadability();
		}

		template <typename T, typename F>
		std::future<F> priorityFuturePush(const T &e)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			std::future<F> futur;
			futur = _priority.push(e)->getFuture();
			releaseReadability();
			return futur;
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> priorityFutureEmplace(Args ...args)
		{
			std::call_once(_onceFlag, [&](){
				_publisherThreadId = std::this_thread::get_id().hash();
			});
			// Assure you that you will not call unprotected functions from different thread	
			assert(std::this_thread::get_id().hash() == _publisherThreadId);
			std::future<F> futur;
			futur = (_priority.emplace<T>(args...))->getFuture();
			releaseReadability();
			return futur;
		}

		template <typename T>
		void safePriorityPush(const T& e)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push(e);
			}
			releaseReadability();
		}

		template <typename T, typename ...Args>
		void safePriorityEmplace(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.emplace<T>(args...);
			}
			releaseReadability();
		}

		template <typename T, typename F>
		std::future<F> safePriorityFuturePush(const T &e)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			std::future<F> futur;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				futur = _priority.push(e)->getFuture();
			}
			releaseReadability();
			return futur;
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> safePriorityFutureEmplace(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			assert(_publisherThreadId == 0);
			std::future<F> futur;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				futur = (_priority.emplace<T>(args...))->getFuture();
			}
			releaseReadability();
			return futur;
		}

		template <typename T>
		void autoPriorityPush(const T& e)
		{
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push(e);
			}
			else
			{
				_queue.push(e);
			}
			releaseReadability();
		}

		template <typename T, typename ...Args>
		void autoPriorityEmplace(Args ...args)
		{
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.emplace<T>(args...);
			}
			else
			{
				_queue.emplace<T>(args...);
			}
			releaseReadability();
		}

		template <typename T, typename F>
		std::future<F> autoPriorityFuturePush(const T &e)
		{
			std::future<F> futur;
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				futur = _priority.push(e)->getFuture();
			}
			else
				futur = _priority.push(e)->getFuture();
			releaseReadability();
			return futur;
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> autoPriorityFutureEmplace(Args ...args)
		{
			std::future<F> futur;
			// Assure you that you didn't call unprotected function before
			if (_publisherThreadId == 0)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				futur = (_priority.emplace<T>(args...))->getFuture();
			}
			else
				futur = (_priority.emplace<T>(args...))->getFuture();
			releaseReadability();
			return futur;
		}

	};
}