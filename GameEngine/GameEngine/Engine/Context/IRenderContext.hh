
#ifndef IRenderContext_HH_
#define IRenderContext_HH_

#include "Core/Input.hh"

class IRenderContext
{
public:
	IRenderContext() { }
	virtual ~IRenderContext() { }

	virtual bool start(unsigned int swidth, unsigned int sheight, const char *name) = 0;
	virtual void updateEvents(Input &input) const = 0;
	virtual void flush() const = 0;
	virtual void stop() const = 0;
};

#endif