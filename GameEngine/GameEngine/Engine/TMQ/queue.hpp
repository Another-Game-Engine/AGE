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
		PtrQueue(const TMQ::PtrQueue &o);
		PtrQueue& operator=(const TMQ::PtrQueue &o);
		PtrQueue(std::size_t chunkSize = 1024);
		PtrQueue& operator=(TMQ::PtrQueue &&o);
		PtrQueue(TMQ::PtrQueue &&o);
		~PtrQueue();
		void pop();
		MessageBase *front();
		std::size_t getFrontSize();
		void clear();
		void release();
		bool empty();
	private:
		std::size_t _chunkSize;
		char *_data;
		std::size_t _cursor;
		std::size_t _size;
		std::size_t _to;

		friend class Queue;

	private:
		template <typename T>
		T* push(const T& e)
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
			Message<T>* res = new(tmp)Message<T>(std::move(e));
			_to += sizeOfInt + s;
			return &res->_data;
		}

		void move(MessageBase *e, std::size_t size)
		{
			std::size_t s = size;
			std::size_t sizeOfInt = sizeof(std::size_t);

			if (_data == nullptr
				|| _size - _to < s + sizeOfInt)
			{
				allocate(size);
			}

			char *tmp = _data;
			tmp += _to;
			memcpy(tmp, &s, sizeOfInt);
			tmp += sizeOfInt;
			e->clone(tmp);
			auto test = (MessageBase*)(tmp);
			_to += sizeOfInt + s;
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

		void allocate(std::size_t sizeOfType)
		{
			while (_size - _to <= sizeOfType + sizeof(std::size_t)
				|| _size <= _to)
			{
				_size += _chunkSize;
				_data = (char*)(realloc(_data, _size));
			}
		}
	};

	class Queue
	{
		PtrQueue _queue;
		PtrQueue _copy;
		std::mutex _mutex;
		std::condition_variable _readCondition;
		std::condition_variable _writeCondition;
		std::size_t _publisherThreadId;
		std::once_flag _onceFlag;
		std::size_t _millisecondToWait;
		const bool _shared; // true if shared, false if only one thread push in it
	public:
		Queue(bool shared);
		void launch();

		Queue(const Queue&) = delete;
		Queue &operator=(const Queue&) = delete;
		Queue(Queue &&) = delete;
		Queue operator=(Queue &&) = delete;

		bool getReadableQueue(TMQ::PtrQueue &q);
		Dispatcher getDispatcher();
		void releaseReadability();
		bool isWritable();
		void setWaitingTime(std::size_t milliseconds);
		std::size_t getWaitingTime();

		//////
		////// Internal standard queue access

		//Lock mutex or not
		//Depend of the usage you made before
		void move(MessageBase *e, std::size_t size)
		{
			if (_shared)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.move(e, size);
				return;
			}
			else
			{
				std::call_once(_onceFlag, [&](){
					_publisherThreadId = std::this_thread::get_id().hash();
				});
				// Assure you that you will not call unprotected functions from different thread	
				assert(std::this_thread::get_id().hash() == _publisherThreadId);
			}
			_queue.move(e, size);
		}


		template <typename T>
		void push(const T& e)
		{
			// Assure you that you didn't call unprotected function before
			if (_shared)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push(e);
				return;
			}
			_queue.push(e);
		}

		template <typename T, typename ...Args>
		void emplace(Args ...args)
		{
			if (_shared)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.emplace<T>(args...);
				return;
			}
			else
			{
				std::call_once(_onceFlag, [&](){
					_publisherThreadId = std::this_thread::get_id().hash();
				});
				// Assure you that you will not call unprotected functions from different thread
				assert(std::this_thread::get_id().hash() == _publisherThreadId);
			}
			_queue.emplace<T>(args...);
		}

		template <typename T, typename F>
		std::future<F> pushFuture(const T &e)
		{
			if (_shared)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				return _queue.push(e)->getFuture();
			}
			else
			{
				std::call_once(_onceFlag, [&](){
					_publisherThreadId = std::this_thread::get_id().hash();
				});
				// Assure you that you will not call unprotected functions from different thread
				assert(std::this_thread::get_id().hash() == _publisherThreadId);
			}
			return _queue.push(e)->getFuture();
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFuture(Args ...args)
		{
			if (_shared)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				return _queue.emplace<T>(args...)->getFuture();
			}
			else
			{
				std::call_once(_onceFlag, [&](){
					_publisherThreadId = std::this_thread::get_id().hash();
				});
				// Assure you that you will not call unprotected functions from different thread	
				assert(std::this_thread::get_id().hash() == _publisherThreadId);
			}
			return _queue.emplace<T>(args...)->getFuture();
		}
	};
}