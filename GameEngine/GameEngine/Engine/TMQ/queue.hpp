#pragma once

#include "message.hpp"

#include <cstdlib>
#include <new>
#include <utility>
#include <cinttypes>
#include <condition_variable>
#include <mutex>
#include <list>

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
		bool empty();
	private:
		std::size_t _chunkSize;
		struct Chunk
		{
			char *_data;
			std::size_t _cursor;
			std::size_t _size;
			std::size_t _to;

			Chunk() = delete;
			Chunk(const Chunk &o) = delete;
			Chunk& operator=(const Chunk &o) = delete;
			Chunk(std::size_t chunkSize);
			Chunk& operator=(Chunk &&o);
			Chunk(Chunk &&o);
			~Chunk();
			void pop();
			MessageBase *front();
			std::size_t getFrontSize();
			void clear();
			void eraseAll();
			void release();
			bool empty();

			template <typename T>
			T* push(const T& e)
			{
				std::size_t sizeOfInt = sizeof(std::size_t);
				std::size_t s = sizeof(Message<T>);
				assert(s < _chunkSize + sizeOfInt);
				
				if (_size - _to < s + sizeOfInt)
				{
					return nullptr;
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

				if (_size - _to < s + sizeOfInt)
				{
					return nullptr;
				}

				char *tmp = _data;
				tmp += _to;
				memcpy(tmp, &s, sizeOfInt);
				tmp += sizeOfInt;
				Message<T>* res = new(tmp)Message<T>(std::move(e));
				_to += sizeOfInt + s;
				return &res->_data;
			}

			bool move(MessageBase *e, std::size_t size)
			{
				std::size_t s = size;
				std::size_t sizeOfInt = sizeof(std::size_t);

				if (_size - _to < s + sizeOfInt)
				{
					return false;
				}

				char *tmp = _data;
				tmp += _to;
				memcpy(tmp, &s, sizeOfInt);
				tmp += sizeOfInt;
				e->clone(tmp);
				auto test = (MessageBase*)(tmp);
				_to += sizeOfInt + s;
				return true;
			}

			template <typename T, typename ...Args>
			T* emplace(Args ...args)
			{
				assert(sizeof(T) % 4 == 0);
				std::size_t s = sizeof(Message<T>);
				std::size_t sizeOfInt = sizeof(std::size_t);

				if (_size - _to < s + sizeOfInt)
				{
					return nullptr;
				}

				char *tmp = _data;
				tmp += _to;
				memcpy(tmp, &s, sizeOfInt);
				tmp += sizeOfInt;
				Message<T>* res = new(tmp)Message<T>(args...);
				_to += sizeOfInt + s;
				return &res->_data;
			}
		};
		std::list<Chunk*> _list;
		std::list<Chunk*>::iterator _listReader;
		std::list<Chunk*>::iterator _listWriter;

		template <typename T>
		T* push(const T& e)
		{
			assert(sizeof(T) % 4 == 0);
			std::size_t s = sizeof(Message<T>);
			std::size_t sizeOfInt = sizeof(std::size_t);
			assert(s + sizeOfInt < _chunkSize);

			auto res = (*_listWriter)->push(e);
			if (!res)
			{
				if (_listWriter == --std::end(_list))
				{
					_list.push_back(new Chunk(_chunkSize));
					_listWriter = --std::end(_list);
				}
				else
					++_listWriter;
				return push(e);
			}
		}

		template <typename T>
		T* push(T&& e)
		{
			assert(sizeof(T) % 4 == 0);
			std::size_t s = sizeof(Message<T>);
			std::size_t sizeOfInt = sizeof(std::size_t);
			assert(s + sizeOfInt < _chunkSize);

			auto res = (*_listWriter)->push(std::move(e));
			if (!res)
			{
				if (_listWriter == --std::end(_list))
				{
					_list.push_back(new Chunk(_chunkSize));
					_listWriter = --std::end(_list);
				}
				else
					++_listWriter;
				return push(e);
			}
		}
		
		bool move(MessageBase *e, std::size_t size)
		{
			std::size_t s = size;
			std::size_t sizeOfInt = sizeof(std::size_t);
			assert(s + sizeOfInt < _chunkSize);

			if ((*_listWriter)->move(std::move(e), size))
				return true;
			if (_listWriter == --std::end(_list))
			{
				_list.push_back(new Chunk(_chunkSize));
				_listWriter = --std::end(_list);
			}
			else
				++_listWriter;
			return (*_listWriter)->move(std::move(e), size);
		}

		template <typename T, typename ...Args>
		T* emplace(Args ...args)
		{
			assert(sizeof(T) % 4 == 0);
			std::size_t s = sizeof(Message<T>);
			std::size_t sizeOfInt = sizeof(std::size_t);
			assert(s + sizeOfInt < _chunkSize);

			auto res = (*_listWriter)->emplace<T>(args...);
			if (!res)
			{
				if (_listWriter == --std::end(_list))
				{
					_list.push_back(new Chunk(_chunkSize));
					_listWriter = --std::end(_list);
				}
				else
					++_listWriter;
				return emplace<T>(args...);
			}
			return res;
		}
		friend class ReleasableQueue;
		friend class ImmediateQueue;
		friend class HybridQueue;
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
			, NoWait = 1
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