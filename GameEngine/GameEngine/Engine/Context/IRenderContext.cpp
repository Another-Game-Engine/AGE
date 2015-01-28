#include "IRenderContext.hh"
#include "Core/Input.hh"
#include <tmq/message.hpp>
#include <Configuration.hpp>

#ifdef USE_IMGUI
#include <Utils/Age_Imgui.hpp>
#endif

#include <Core/CullableObjects.hh>

namespace AGE
{
	IRenderContext::IRenderContext()
		: _screenSize(glm::uvec2(10, 10))
		, _windowName("<3 ~AGE~ <3")
	{ }

	IRenderContext::~IRenderContext()
	{
	}

	bool IRenderContext::init(int mode, unsigned int swidth, unsigned int sheight, std::string && name)
	{
		_windowName = name;
		_screenSize = glm::uvec2(swidth, sheight);
		return _init(mode);
	}

	const glm::uvec2 &IRenderContext::getScreenSize() const { return _screenSize; }
}