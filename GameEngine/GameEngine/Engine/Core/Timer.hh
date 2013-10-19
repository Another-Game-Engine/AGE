
#ifndef TIMER_HH_
#define TIMER_HH_

class Timer
{
private:
	unsigned int 		_oldTime;
	unsigned int		_curTime;

public:
	Timer();
	~Timer() { }

	void 		update();
	double 		getElapsed() const;
};

#endif