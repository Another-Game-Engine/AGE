
#ifndef IRenderContext_HH_
#define IRenderContext_HH_

#include "Core/Input.hh"
#include <Utils/Dependency.hpp>

class IRenderContext : public Dependency
{
public:
	IRenderContext()
		: _width(0)
		, _height(0)
	{ }
	virtual ~IRenderContext() { }

	virtual bool start(unsigned int swidth, unsigned int sheight, const char *name) = 0;
	virtual void updateEvents(Input &input) const = 0;
	virtual void flush() const = 0;
	virtual void stop() const = 0;
	glm::uvec2 getScreenSize() const
	{
		return glm::uvec2(_width, _height);
	}
protected:
	unsigned int     _width;
	unsigned int     _height;
};

#endif