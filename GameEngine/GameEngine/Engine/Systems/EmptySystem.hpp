#ifndef   __EMPTY_SYSTEM_HPP__
# define  __EMPTY_SYSTEM_HPP__

#include "System.h"

class EmptySystem : public System
{
public:
	EmptySystem(){}
	virtual ~EmptySystem(){}
private:
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{}

	virtual void initialize()
	{}
};

#endif    //__EMPTY_SYSTEM_HPP__