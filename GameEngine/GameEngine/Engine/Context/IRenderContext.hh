
#ifndef IRenderContext_HH_
#define IRenderContext_HH_

#include "Core/Input.hh"
#include <Utils/Dependency.hpp>
#include <Utils/CommandQueueHolder.hpp>
#include <tmq/message.hpp>
#include <Configuration.hpp>

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

#ifdef USE_IMGUI
	struct ::ImDrawList;
	struct RenderImgui
	{
		::ImDrawList** const cmd_lists; int cmd_lists_count;
		RenderImgui(::ImDrawList** const _cmd_lists, int _cmd_lists_count)
			: cmd_lists(_cmd_lists)
			, cmd_lists_count(_cmd_lists_count)
		{}
	};
#endif
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