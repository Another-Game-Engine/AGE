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
	class ReleasableQueue;
	class Dispatcher;
	class Messsage;
	class ImmediateQueue;

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
		void eraseAll();
		void release();
		bool empty();
	private:
		std::size_t _chunkSize;
		char *_data;
		std::size_t _cursor;
		std::size_t _size;
		std::size_t _to;

		friend class ReleasableQueue;
		friend class ImmediateQueue;

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

	class ImmediateQueue
	{
	private:
		PtrQueue _queue;
		std::mutex _mutex;
		std::condition_variable _condition;
		bool _releaseReadability()
		{
			_condition.notify_one();
			return true;
		}
	public:
		ImmediateQueue()
		{}

		ImmediateQueue(const ImmediateQueue&) = delete;
		ImmediateQueue &operator=(const ImmediateQueue&) = delete;
		ImmediateQueue(ImmediateQueue &&) = delete;
		ImmediateQueue operator=(ImmediateQueue &&) = delete;

		bool getReadableQueue(TMQ::PtrQueue &q)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (!_condition.wait_for(lock, std::chrono::milliseconds(1), [this](){ return !_queue.empty(); }))
				return false;
			q = std::move(_queue);
			_queue.clear();
			lock.unlock();
			return true;
		}

		void move(MessageBase *e, std::size_t size)
		{
			{
				std::unique_lock<std::mutex> lock(_mutex);
				_queue.move(e, size);
			}
			_releaseReadability();
		}

		template <typename T>
		void push(const T& e)
		{
			{
				std::unique_lock<std::mutex> lock(_mutex);
				_queue.push(e);
			}
			_releaseReadability();
		}

		template <typename T, typename ...Args>
		void emplace(Args ...args)
		{
			{
				std::unique_lock<std::mutex> lock(_mutex);
				_queue.emplace<T>(args...);
			}
			_releaseReadability();
		}

		template <typename T, typename F>
		std::future<F> pushFuture(const T &e)
		{
			std::future < F > f;
			{
				std::unique_lock<std::mutex> lock(_mutex);
				f = _queue.push(e)->getFuture();
			}
			_releaseReadability();
			return f;
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFuture(Args ...args)
		{
			std::future< F > f;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				f = _queue.emplace<T>(args...)->getFuture();
			}
			_releaseReadability();
			return f;
		}
	};

	class ReleasableQueue
	{
		PtrQueue _queue;
		PtrQueue _copy;
		std::mutex _mutex;
		std::condition_variable _readCondition;
		std::condition_variable _writeCondition;
		std::size_t _millisecondToWait;
	public:
		ReleasableQueue();
		void launch();

		ReleasableQueue(const ReleasableQueue&) = delete;
		ReleasableQueue &operator=(const ReleasableQueue&) = delete;
		ReleasableQueue(ReleasableQueue &&) = delete;
		ReleasableQueue operator=(ReleasableQueue &&) = delete;

		bool getReadableQueue(TMQ::PtrQueue &q);
		Dispatcher getDispatcher();
		bool releaseReadability(bool wait = true);
		bool isWritable();
		void setWaitingTime(std::size_t milliseconds);
		std::size_t getWaitingTime();
		void clear();

		//////
		////// Internal standard queue access

		//Lock mutex or not
		//Depend of the usage you made before
		void move(MessageBase *e, std::size_t size)
		{
			_queue.move(e, size);
		}

		template <typename T>
		void push(const T& e)
		{
			_queue.push(e);
		}

		template <typename T, typename ...Args>
		void emplace(Args ...args)
		{
			_queue.emplace<T>(args...);
		}

		template <typename T, typename F>
		std::future<F> pushFuture(const T &e)
		{
			return _queue.push(e)->getFuture();
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFuture(Args ...args)
		{
			return _queue.emplace<T>(args...)->getFuture();
		}
	};
}