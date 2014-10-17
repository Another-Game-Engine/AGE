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