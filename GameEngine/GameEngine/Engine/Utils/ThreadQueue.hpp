#pragma once

#include <thread>
#include <tmq/doubleBuffered/templateDispatcher.hpp>
#include <tmq/doubleBuffered/queue.hpp>

namespace AGE
{
	class ThreadQueue
	{
	private:
		std::thread _thread;
		TMQ::Double::Queue _commandQueue;
	protected:
		template<typename _Message_2, typename _Function_2>
		virtual bool _update(TemplateDispatcher<TemplateDispatcher, _Message_2, _Function_2>) = 0;
	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		ThreadQueue()
		{}

		virtual ~ThreadQueue()
		{
			_thread.join();
		}

		bool update()
		{
			bool run = true;
			while (run)
			{
				_update()
			}
		}

		void launch()
		{
			std::thread(&ThreadQueue::update, std::ref(*this));
		}
	};
}