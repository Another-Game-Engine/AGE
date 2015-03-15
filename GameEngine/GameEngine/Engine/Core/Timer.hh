#pragma once

#include <Utils/Dependency.hpp>
#include <chrono>

namespace AGE
{
	class Timer : public Dependency < Timer >
	{
	private:
		std::chrono::system_clock::time_point 		_oldTime;
		std::chrono::system_clock::time_point		_curTime;
		float _elapsed;

	public:
		Timer();
		virtual ~Timer() { }

		void 		update();
		float       getElapsed() const;
		float 		getNow() const;
	};
}