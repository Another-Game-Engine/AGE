#pragma once

#include <Utils/Dependency.hpp>
#include <tmq/templateDispatcher.hpp>
#include <tmq/queue.hpp>

namespace AGE
{
	class MainThread : public Dependency < MainThread >
	{
	public:

		MainThread()
			: _next(nullptr)
			, _thisThreadId(std::this_thread::get_id().hash())
		{
			_commandQueue.launch();
		}

		// USE ONLY THAT FUNCTION TO GET THE COMMAND QUEUE
		TMQ::Queue *getCommandQueue()
		{
			assert(std::this_thread::get_id().hash() == _thisThreadId);
			return _next;
		}

		void setNextCommandQueue(TMQ::Queue *next)
		{
			_next = next;
		}

		// USE THAT FUNCTION ONLY TO PASS THE RESULT to setNextCommandQueue
		// Do NOT use it to pass messages
		TMQ::Queue *getCurrentThreadCommandQueue()
		{
			return &_commandQueue;
		}
	private:
		TMQ::Queue _commandQueue;
		TMQ::Queue *_next;
		std::size_t _thisThreadId;
	};
}