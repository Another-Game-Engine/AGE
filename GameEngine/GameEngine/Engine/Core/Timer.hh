#pragma once

#include <Utils/Dependency.hpp>

class Timer : public Dependency<Timer>
{
private:
	unsigned int 		_oldTime;
	unsigned int		_curTime;

public:
	Timer();
	virtual ~Timer() { }

	void 		update();
	double 		getElapsed() const;
	double 		getNow() const;
};