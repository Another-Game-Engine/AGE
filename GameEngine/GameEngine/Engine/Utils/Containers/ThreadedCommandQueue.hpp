#pragma once

/////////////
// TODO
////////////

//#include "CommandQueue.hpp"
//
//#include <mutex>
//#include <thread>
//#include <condition_variable>
//#include <atomic>
//
////debug
//#include <iostream>
//
//namespace AGE
//{
//	template <typename Base = ICommandQueue::BaseCommand>
//	class ThreadedCommandQueue : ICommandQueue
//	{
//	public:
//		ThreadedCommandQueue(std::size_t chunkSize = 1024)
//			: _chunkSize(chunkSize)
//			, _thread(nullptr)
//		{
//			_isRunning = false;
//			_thread = new std::thread(&CommandQueue<Base>::worker, std::ref(*this));
//		}
//
//		~ThreadedCommandQueue()
//		{
//			_isRunning = false;
//			_hasSomeWork.notify_one();
//			_thread->join();
//			delete _thread;
//		}
//
//		template <typename T>
//		T* create()
//		{
//			return _mtQueue.create<T>();
//		}
//
//		void execute()
//		{
//			std::unique_lock<std::mutex> lock(_mutex);
//			auto tmp = std::move(_wtQueue);
//			_wtQueue = std::move(_mtQueue);
//			_mtQueue = std::move(tmp);
//
//			lock.unlock();
//			_hasSomeWork.notify_one();
//		}
//	private:
//		void worker()
//		{
//			_isRunning = true;
//			while (true)
//			{
//				std::unique_lock<std::mutex> lock(_mutex);
//				_hasSomeWork.wait(lock, [&]{ return _wt_Data.cursor != 0; });
//				if (!_isRunning)
//				{
//					return;
//				}
//				std::size_t c = 0;
//				char *tmp = _wt_Data.data;
//				std::size_t soi = sizeof(std::size_t);
//
//				while (c != _wt_Data.cursor)
//				{
//					tmp = _wt_Data.data;
//					tmp += c;
//					std::size_t s = *reinterpret_cast<std::size_t*>(tmp);
//					tmp += soi;
//					auto b = ((Base*)(tmp));
//					b->execute();
//					tmp += s;
//					c += s + soi;
//				}
//				_wt_Data.cursor = 0;
//			}
//		}
//
//		template <typename T>
//		void allocate()
//		{
//			std::size_t sizeOfType = sizeof(T);
//			
//			while (_data.size - _data.cursor <= sizeOfType + sizeof(std::size_t)
//				|| _data.size <= _data.cursor)
//			{
//				_data.size += _chunkSize;
//				_data.data = (char*)(realloc(_data.data, _data.size));
//			}
//		}
//	private:
//		struct DataHolder
//		{
//			char *data;
//			std::size_t cursor;
//			std::size_t size;
//			DataHolder()
//				: data(nullptr)
//				, cursor(0)
//				, size(0)
//			{}
//
//			DataHolder(DataHolder &&o)
//			{
//				data = std::move(o.data);
//				cursor = std::move(o.cursor);
//				size = std::move(o.size);
//				o.data = nullptr;
//			}
//
//			DataHolder&& operator=(DataHolder &&o)
//			{
//				data = std::move(o.data);
//				cursor = std::move(o.cursor);
//				size = std::move(o.size);
//				o.data = nullptr;
//				return std::move(*this);
//			}
//
//			~DataHolder()
//			{
//				if (data != nullptr)
//					free(data);
//			}
//		};
//
//		std::size_t _chunkSize;
//
//		std::thread *_thread;
//		std::atomic_bool _isRunning;
//		std::mutex _mutex;
//		std::condition_variable _hasSomeWork;
//
//		AGE::CommandQueue _mtData;
//		AGE::CommandQueue _wtData;
//	};
//}