
#include "Timer.hh"

#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>

#if  !WIN32
#include <sys/time.h>
#else
#include <ctime>
#endif

namespace AGE
{
	Timer::Timer() :
		_elapsed(0.0l)
	{
		_oldTime = std::chrono::system_clock::now();
		_curTime = std::chrono::system_clock::now();
	}

	void 		Timer::update()
	{
		_oldTime = _curTime;
		_curTime = std::chrono::system_clock::now();
		_elapsed = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(_curTime - _oldTime).count()) / 1000.0l;
	}

	double 		Timer::getElapsed() const
	{
		return _elapsed;
	}

	double 		Timer::getNow() const
	{
		return (std::chrono::duration_cast<std::chrono::milliseconds>(_curTime.time_since_epoch()).count() / 1000.0l);
	}
}