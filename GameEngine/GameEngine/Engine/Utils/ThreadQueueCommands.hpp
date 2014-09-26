#pragma once

#include <tmq/message.hpp>

namespace AGE
{
	namespace TQC // == ThreadQueueCommands
	{
		struct BoolFunction : public TMQ::FutureData < bool >
		{
			std::function<bool()> function;
			BoolFunction(const std::function<bool()> &_function)
				: function(_function)
			{
			}
			virtual ~BoolFunction()
			{}
		};

		//Call that in first for each thread
		struct StartOfFrame
		{
			//std::chrono::system_clock::time_point ms;
			//StartOfFrame()
			//{
			//	ms = std::chrono::system_clock::now();
			//}
		};

		struct EndOfFrame
		{
			//EndOfFrame()
			//{
			//}
		};

		struct VoidFunction
		{
			std::function<void(void)> function;
			VoidFunction(const std::function<void(void)> &_function)
				: function(_function)
			{
			}
			VoidFunction(const VoidFunction &) = delete;
			VoidFunction &operator=(const VoidFunction&) = delete;
			~VoidFunction()
			{
			}
		};
	}
}