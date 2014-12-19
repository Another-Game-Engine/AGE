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
	class HybridQueue;

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
		friend class HybridQueue;

	private:
		template <typename T>
		T* push(const T& e)
		{
			assert(sizeof(T) % 4 == 0);
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
			assert(sizeof(T) % 4 == 0);
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
			assert(sizeof(T) % 4 == 0);
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
			assert(sizeof(T) % 4 == 0);
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

class HybridQueue
	{
		PtrQueue _commandQueue;
		PtrQueue _commandQueueCopy;
		PtrQueue _taskQueue;
		std::mutex _mutex;
		std::condition_variable _readCondition;
		std::condition_variable _writeCondition;
		std::size_t _millisecondToWait;
		std::atomic_bool _releasable;
		std::size_t _reservedPublisher;
	public:
		enum WaitType
		{
			Block = 0
			, Wait = 1
			, NoWait = 2
		};

		HybridQueue();
		void launch();

		HybridQueue(const HybridQueue&) = delete;
		HybridQueue &operator=(const HybridQueue&) = delete;
		HybridQueue(HybridQueue &&) = delete;
		HybridQueue &operator=(HybridQueue &&) = delete;

		bool getTaskQueue(TMQ::PtrQueue &q, WaitType waitType);
		void getTaskAndCommandQueue(
			TMQ::PtrQueue &taskQueue,
			bool &taskQueueSuccess,
			TMQ::PtrQueue &commandQueue,
			bool &commandQueueSuccess,
			WaitType waitType);
		bool releaseTaskReadability();
		bool releaseCommandReadability(WaitType waitType);
		void setWaitingTime(std::size_t milliseconds);
		std::size_t getWaitingTime();
		void clear();
		inline void reserveTo(std::size_t publisherThreadId) { _reservedPublisher = publisherThreadId; }

		//////
		////// Internal standard queue access

		/////////
		/// COMMANDS
		void moveCommand(MessageBase *e, std::size_t size)
		{
			assert(_reservedPublisher != -1 && std::this_thread::get_id().hash() == _reservedPublisher);
			_commandQueue.move(e, size);
		}

		template <typename T>
		void pushCommand(const T& e)
		{
			assert(_reservedPublisher != -1 && std::this_thread::get_id().hash() == _reservedPublisher);
			_commandQueue.push(e);
		}

		template <typename T, typename ...Args>
		void emplaceCommand(Args ...args)
		{
			assert(_reservedPublisher != -1 && std::this_thread::get_id().hash() == _reservedPublisher);
			_commandQueue.emplace<T>(args...);
		}

		template <typename T, typename F>
		std::future<F> pushFutureCommand(const T &e)
		{
			assert(_reservedPublisher != -1 && std::this_thread::get_id().hash() == _reservedPublisher);
			return _commandQueue.push(e)->getFuture();
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFutureCommand(Args ...args)
		{
			assert(_reservedPublisher != -1 && std::this_thread::get_id().hash() == _reservedPublisher);
			return _commandQueue.emplace<T>(args...)->getFuture();
		}

		//////////////
		//// TASKS

		void moveTask(MessageBase *e, std::size_t size)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_taskQueue.move(e, size);
			}
			releaseTaskReadability();
		}

		template <typename T>
		void pushTask(const T& e)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_taskQueue.push(e);
			}
			releaseTaskReadability();
		}

		template <typename T, typename ...Args>
		void emplaceTask(Args ...args)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_taskQueue.emplace<T>(args...);
			}
			releaseTaskReadability();
		}

		template <typename T, typename F>
		std::future<F> pushFutureTask(const T &e)
		{
			std::future < F > f;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				f = _taskQueue.push(e)->getFuture();
			}
			releaseTaskReadability();
			return f;
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFutureTask(Args ...args)
		{
			std::future< F > f;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				f = _taskQueue.emplace<T>(args...)->getFuture();
			}
			releaseTaskReadability();
			return f;
		}
	};

}