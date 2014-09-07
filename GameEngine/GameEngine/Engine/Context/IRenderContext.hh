
#ifndef IRenderContext_HH_
#define IRenderContext_HH_

#include "Core/Input.hh"
#include <Utils/Dependency.hpp>
#include <Utils/CommandQueueHolder.hpp>
#include <tmq/common/message.hpp>

namespace RendCtxCommand
{
	struct Flush{};
	struct SetScreenSize
	{
		SetScreenSize(const glm::uvec2& v)
			: screenSize(v)
		{}
		glm::uvec2 screenSize;
	};
	struct GetScreenSize : public TMQ::FutureData < glm::uvec2 >
	{};

	struct RefreshInputs
	{};
}

class IRenderContext : public Dependency<IRenderContext>
{
public:
	IRenderContext()
		: _screenSize(glm::uvec2(10,10))
		, _windowName("<3 ~AGE~ <3")
	{ }
	virtual ~IRenderContext()
	{
	}

	bool init(int mode, unsigned int swidth, unsigned int sheight, std::string && name)
	{
		_windowName = name;
		_screenSize = glm::uvec2(swidth, sheight);
		return _init(mode);
	}

protected:
	glm::uvec2 _screenSize;
	std::string _windowName;
	virtual bool _init(int mode) = 0;
};

#endif