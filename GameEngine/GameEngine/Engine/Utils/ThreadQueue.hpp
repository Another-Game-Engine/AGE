#pragma once

#include <thread>
#include <Utils/CommandQueue.hpp>
#include <Utils/ThreadName.hpp>
#include <iostream>
#include <string>
#include <Core/MainThreadCommand.hpp>

namespace AGE
{
	class ThreadQueue : public CommandQueue
	{
	protected:
		std::thread _thread;
		std::atomic_bool _run;
		bool _lastOfLoop;

		bool runFunction(Engine *engine);
	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		ThreadQueue();
		virtual ~ThreadQueue();
		bool launch(Engine *engine, const std::string &name);
		void quit();
	};
}