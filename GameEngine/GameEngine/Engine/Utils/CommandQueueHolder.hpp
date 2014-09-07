#pragma once

//#include <tmq/doubleBuffered/queue.hpp>
//#include <tmq/doubleBuffered/templateDispatcher.hpp>
//
//namespace AGE
//{
//	class CommandQueueHolder
//	{
//	protected:
//		TMQ::Double::Queue _commandQueue;
//	public:
//		virtual ~CommandQueueHolder(){}
//		virtual bool updateCommandQueue() = 0;
//		inline TMQ::Double::Queue &getCommandQueue()
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