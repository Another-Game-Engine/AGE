#pragma once

//#include <tmq/queue.hpp>
//#include <tmq/templateDispatcher.hpp>
//
//namespace AGE
//{
//	class CommandQueueHolder
//	{
//	protected:
//		TMQ::Queue _commandQueue;
//	public:
//		virtual ~CommandQueueHolder(){}
//		virtual bool updateCommandQueue() = 0;
//		inline TMQ::Queue &getCommandQueue()
//		{
//			return _commandQueue;
//		}
//
//		//Should be called in sender thread, not in worker one.
//		inline void launchCommandQueue()
//		{
//			_commandQueue.launch();
//		}
//	};
//}