#pragma once

#include "message.hpp"

#include <cstdlib>
#include <new>
#include <utility>
#include <cinttypes>
#include <condition_variable>
#include <mutex>
#include <list>
#include <Utils/Profiler.hpp>

#include <queue>

// tmp
#include "Utils/Debug.hpp"

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
		static std::queue<MessageBase*>              _sharedQueue;
		std::queue<MessageBase*>                     _individualQueue;
		static std::mutex              _mutex;
		static std::condition_variable _condition;
	public:
		HybridQueue();

		HybridQueue(const HybridQueue&) = delete;
		HybridQueue &operator=(const HybridQueue&) = delete;
		HybridQueue(HybridQueue &&) = delete;
		HybridQueue &operator=(HybridQueue &&) = delete;

		void getTask(MessageBase *& task);
		void tryToGetTask(MessageBase *& task, std::size_t microSeconds);

		//////////////
		//// TASKS

		template <typename T>
		void pushTask(const T& e)
		{
			SCOPE_profile_cpu_function("TMQ");
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_individualQueue.push(new Message<T>(e));
			}
			_condition.notify_all();
		}

		template <typename T, typename ...Args>
		void emplaceTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_individualQueue.push(new Message<T>(args...));
			}
			_condition.notify_all();
		}

		template <typename T, typename F>
		std::future<F> pushFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			auto tmp = new Message<T>(e);
			f = tmp->getData().getFuture();
			std::future < F > f;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_individualQueue.push(tmp);
			}
			_condition.notify_all();
			return f;
		}

		template <typename T, typename F, typename ...Args>
		std::future<F> emplaceFutureTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future< F > f;
			auto tmp = new Message<T>(args...);
			f = tmp->getData().getFuture();
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_individualQueue.push(tmp);
			}
			_condition.notify_all();
			return f;
		}


		///////
		// SHARED

		template <typename T>
		static void pushSharedTask(const T& e)
		{
			SCOPE_profile_cpu_function("TMQ");
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_sharedQueue.push(new Message<T>(e));
			}
			_condition.notify_all();
		}

		template <typename T, typename ...Args>
		static void emplaceSharedTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_sharedQueue.push(new Message<T>(args...));
			}
			_condition.notify_all();
		}

		template <typename T, typename F>
		static std::future<F> pushSharedFutureTask(const T &e)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future < F > f;
			auto tmp = new Message<T>(e);
			f = tmp->getData().getFuture();
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_sharedQueue.push(tmp);
			}
			_condition.notify_all();
			return f;
		}

		template <typename T, typename F, typename ...Args>
		static std::future<F> emplaceSharedFutureTask(Args... args)
		{
			SCOPE_profile_cpu_function("TMQ");
			std::future< F > f;
			auto tmp = new Message<T>(args...);
			f = tmp->getData().getFuture();
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_sharedQueue.push(tmp);
			}
			_condition.notify_all();
			return f;
		}

	};

}
