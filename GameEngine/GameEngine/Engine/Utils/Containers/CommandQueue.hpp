#pragma once

#include <cstdlib>
#include <new>

#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace AGE
{
	struct BaseCommand
	{
		virtual void execute() = 0;		
	};

	struct TestCommand
	{
		virtual void execute()
		{}

		char a = 'A';
		int b = 42;
		void *d = nullptr;
	};

	template <typename Base = BaseCommand>
	class CommandQueue
	{
	public:
		CommandQueue(std::size_t chunkSize = 1024)
			: _data(nullptr)
			, _cursor(0)
			, _size(0)
			, _chunkSize(chunkSize)
			, _thread(nullptr)
		{
			_isRunning = false;
			_thread = new std::thread(&CommandQueue<Base>::worker, std::ref(*this));
		}

		~CommandQueue()
		{
			_isRunning = false;
			_hasSomeWork.notify_one();
			_thread->join();
			delete _thread;
			if (_data != nullptr)
				free(_data);
		}

		template <typename T>
		T* create()
		{
			if (_data == nullptr || _size - _cursor < sizeof(T)+sizeof(std::size_t))
			{
				allocate<T>();
			}
			std::size_t s = sizeof(T);

			char *tmp = _data;
			tmp += _cursor;
			memcpy(tmp, &s, sizeof(std::size_t));
			tmp += sizeof(std::size_t);
			T* res = new(tmp)T();
			_cursor += sizeof(std::size_t) + sizeof(T);
			return res;
		}

		void execute()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_workerData = std::move(_data);
			_workerCursor = _cursor;
			_cursor = 0;
			lock.unlock();
			_hasSomeWork.notify_one();
		}
	private:
		void worker()
		{
			_isRunning = true;
			while (_isRunning)
			{
				std::unique_lock<std::mutex> lock(_mutex);
				_hasSomeWork.wait(lock);
				if (!_isRunning)
				{
					lock.unlock();
					return;
				}

				std::size_t c = 0;
				char *tmp = _workerData;
				std::size_t soi = sizeof(std::size_t);

				std::size_t test = 0;

				while (c != _workerCursor)
				{
					tmp = _workerData;
					tmp += c;
					std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
					tmp += soi;
					auto b = ((Base*)(tmp));
					b->execute();
					tmp += s;
					c += s + soi;
					std::cout << test << std::endl;
					++test;
				}
				lock.unlock();
			}
		}

		template <typename T>
		void allocate()
		{
			std::size_t sizeOfType = sizeof(T);
			
			while (_size - _cursor <= sizeOfType + sizeof(std::size_t) || _size <= _cursor)
			{
				_size += _chunkSize;
				_data = (char*)(realloc(_data, _size));
			}
		}
	private:
		char *_data;
		std::size_t _cursor;
		std::size_t _size;
		std::size_t _chunkSize;

		std::thread *_thread;
		std::atomic_bool _isRunning;
		std::mutex _mutex;
		std::condition_variable _hasSomeWork;
		char *_workerData;
		std::size_t _workerCursor;
	};
}