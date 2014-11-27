#pragma once

#include <string>

namespace AGE
{
	namespace MTC
	{
		struct ThreadRegistration
		{
			ThreadRegistration(std::size_t _id, std::string _name)
				: id(_id)
				, name(_name)
			{
			}
			std::size_t id;
			std::string name;
		};

		struct FrameTime
		{
			FrameTime(std::size_t _id, float _time)
				: id(_id)
				, time(_time)
			{
			}
			std::size_t id;
			unsigned int time;
		};
	}
}