#pragma once

#include <thread>
#include <tmq/doubleBuffered/templateDispatcher.hpp>
#include <tmq/doubleBuffered/queue.hpp>
#include <Core/Engine.hh>
namespace AGE
{
	class ThreadQueue
	{
	private:
		std::thread _thread;
	protected:
		TMQ::Double::Queue _commandQueue;
		virtual bool _update() = 0;
		virtual bool _init() = 0;
		Engine *_engine;

		bool update()
		{
			bool run = true;
			while (run)
			{
				run = _update();
			}
			return true;
		}

	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		ThreadQueue()
			: _engine(nullptr)
		{}

		virtual ~ThreadQueue()
		{
			_thread.join();
		}

		bool launch(Engine *engine)
		{
			_engine = engine;
			assert(_engine != nullptr);
			auto res = _init();
			_commandQueue.launch();
			_thread = std::thread(&ThreadQueue::update, std::ref(*this));
			return res;
		}

		TMQ::Double::Queue &getCommandQueue()
		{
			return _commandQueue;
		}
	};
}