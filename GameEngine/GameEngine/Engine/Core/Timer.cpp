
#include "Timer.hh"

#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>

#if  !WIN32
#include <sys/time.h>
#else
#include <ctime>
#endif

#include <SDL/SDL.h>

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
		_elapsed = std::chrono::duration_cast<std::chrono::seconds>(_curTime - _oldTime).count();
	}

	double 		Timer::getElapsed() const
	{
		return _elapsed;
	}

	double 		Timer::getNow() const
	{
		return static_cast<double>(SDL_GetTicks()) / 1000.0f;
	}
}