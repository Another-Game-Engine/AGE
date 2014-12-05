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
		double _elapsed;

	public:
		Timer();
		virtual ~Timer() { }

		void 		update();
		double 		getElapsed() const;
		double 		getNow() const;
	};
}