
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

Timer::Timer() :
	_oldTime(0),
	_curTime(0)
{
}

void 		Timer::update()
{
	_oldTime = _curTime;
	_curTime = SDL_GetTicks();
	if (_oldTime == 0)
		_oldTime = _curTime;
}

double 		Timer::getElapsed() const
{
	return glm::clamp(static_cast<double>(_curTime - _oldTime) / 1000.0, 0.0, 0.25);
}

