#pragma once

#include <cstdlib>
#include <new>

#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

//debug
#include <iostream>

namespace AGE
{
	struct BaseCommand
	{
		virtual void execute() = 0;		
	};

	struct TestCommand
	{
		virtual void execute()
		{
			std::cout << "Exec : " << a << " n'" << b << std::endl;
		}

		char a = 'A';
		int b = 0;
		void *d = nullptr;
	};

	template <typename Base = BaseCommand>
	class CommandQueue
	{
	public:
		CommandQueue(std::size_t chunkSize = 1024)
			: _chunkSize(chunkSize)
			, _thread(nullptr)
			, _mt_Data(nullptr)
			, _wt_Data(nullptr)
		{
			_isRunning = false;
			_mt_Data = new ThreadData();
			_thread = new std::thread(&CommandQueue<Base>::worker, std::ref(*this));
		}

		~CommandQueue()
		{
			_isRunning = false;
			_hasSomeWork.notify_one();
			_thread->join();
			delete _thread;
			if (_mt_Data != nullptr)
				delete _mt_Data;
			if (_wt_Data != nullptr)
				delete _wt_Data;
		}

		template <typename T>
		T* create()
		{
			if (_mt_Data->data == nullptr
				|| _mt_Data->size - _mt_Data->cursor < sizeof(T)+sizeof(std::size_t))
			{
				allocate<T>();
			}
			std::size_t s = sizeof(T);

			char *tmp = _mt_Data->data;
			tmp += _mt_Data->cursor;
			memcpy(tmp, &s, sizeof(std::size_t));
			tmp += sizeof(std::size_t);
			T* res = new(tmp)T();
			_mt_Data->cursor += sizeof(std::size_t) + sizeof(T);
			return res;
		}

		void execute()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			std::swap(_wt_Data, _mt_Data);
			lock.unlock();
			_hasSomeWork.notify_one();
		}
	private:
		void worker()
		{
			_isRunning = true;
			_wt_Data = new ThreadData();
			while (_isRunning)
			{
				std::unique_lock<std::mutex> lock(_mutex);
				_hasSomeWork.wait_for(lock, std::chrono::milliseconds(10));
				if (!_isRunning)
				{
					lock.unlock();
					return;
				}

				std::size_t c = 0;
				char *tmp = _wt_Data->data;
				std::size_t soi = sizeof(std::size_t);

				while (c != _wt_Data->cursor)
				{
					tmp = _wt_Data->data;
					tmp += c;
					std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
					tmp += soi;
					auto b = ((Base*)(tmp));
					b->execute();
					tmp += s;
					c += s + soi;
				}
				_wt_Data->cursor = 0;
				lock.unlock();
			}
		}

		template <typename T>
		void allocate()
		{
			std::size_t sizeOfType = sizeof(T);
			
			while (_mt_Data->size - _mt_Data->cursor <= sizeOfType + sizeof(std::size_t)
				|| _mt_Data->size <= _mt_Data->cursor)
			{
				_mt_Data->size += _chunkSize;
				_mt_Data->data = (char*)(realloc(_mt_Data->data, _mt_Data->size));
			}
		}
	private:
		struct ThreadData
		{
			char *data;
			std::size_t cursor;
			std::size_t size;
			ThreadData()
				: data(nullptr)
				, cursor(0)
				, size(0)
			{}

			~ThreadData()
			{
				if (data != nullptr)
					free(data);
			}
		};

		std::size_t _chunkSize;

		std::thread *_thread;
		std::atomic_bool _isRunning;
		std::mutex _mutex;
		std::condition_variable _hasSomeWork;

		ThreadData *_mt_Data;
		ThreadData *_wt_Data;
	};
}