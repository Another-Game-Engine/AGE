
#ifndef TIMER_HH_
#define TIMER_HH_

#include <Utils/Dependency.hpp>

class Timer : public Dependency
{
private:
	unsigned int 		_oldTime;
	unsigned int		_curTime;

public:
	Timer();
	virtual ~Timer() { }

	void 		update();
	double 		getElapsed() const;
};

#endif