
#ifndef IRenderContext_HH_
#define IRenderContext_HH_

#include "Core/Input.hh"
#include <Utils/Dependency.hpp>

class IRenderContext : public Dependency
{
public:
	IRenderContext()
		: _screenSize(glm::uvec2(10,10))
		, _windowName("<3 ~AGE~ <3")
	{ }
	virtual ~IRenderContext() { }

	bool start(int mode, unsigned int swidth, unsigned int sheight, std::string && name)
	{
		_windowName = name;
		_screenSize = glm::uvec2(swidth, sheight);
		return _start(mode);
	}

	virtual void updateEvents(Input &input) const = 0;
	virtual void flush() const = 0;
	virtual void stop() const = 0;
	
	const glm::uvec2 &getScreenSize() const
	{
		return _screenSize;
	}

	void setScreenSize(glm::uvec2 &&screenSize)
	{
		_screenSize = screenSize;
		_setScreenSize(std::move(screenSize));
	}

protected:
	glm::uvec2 _screenSize;
	std::string _windowName;
	virtual void _setScreenSize(glm::uvec2 &&screenSize) = 0;
	virtual bool _start(int mode) = 0;
};

#endif