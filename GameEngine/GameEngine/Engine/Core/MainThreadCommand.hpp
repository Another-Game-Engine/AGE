#pragma once

#include <string>

namespace AGE
{
	namespace MTC
	{
		struct FrameTime
		{
			FrameTime(const std::string &_threadName, unsigned int _time)
				: name(_threadName)
				, time(_time)
			{
			}
			std::string name;
			unsigned int time;
		};
	}
}