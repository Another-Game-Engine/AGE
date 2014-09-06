
#ifndef IRenderContext_HH_
#define IRenderContext_HH_

#include "Core/Input.hh"
#include <Utils/Dependency.hpp>
#include <Utils/CommandQueueHolder.hpp>

namespace RendCtxCommand
{
	struct Flush{};
	struct Stop{};
	struct SetScreenSize
	{
		SetScreenSize(const glm::uvec2& v)
			: screenSize(v)
		{}
		glm::uvec2 screenSize;
	};
	struct GetScreenSize : public TMQ::FutureData < glm::uvec2 >
	{};

	struct BoolFunction : public TMQ::FutureData<bool>
	{
		std::function<bool()> function;
		BoolFunction(const std::function<bool()> &_function)
			: function(_function)
		{
		}
	};
}

class IRenderContext : public Dependency<IRenderContext>, public AGE::CommandQueueHolder
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

	virtual bool updateCommandQueue()
	{
		return _update();
	}

	void flush()
	{
 		_commandQueue.emplace<RendCtxCommand::Flush>();
		_commandQueue.releaseReadability();
	}

	void stop()
	{
		_commandQueue.emplace<RendCtxCommand::Stop>();
	}
	
	const glm::uvec2 &getScreenSize()
	{
		auto res = _commandQueue.priorityEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>();
		return res.get();
	}

	void setScreenSize(const glm::uvec2 &screenSize)
	{
		_commandQueue.emplace<RendCtxCommand::SetScreenSize>(screenSize);
	}

protected:
	glm::uvec2 _screenSize;
	std::string _windowName;
	virtual bool _init(int mode) = 0;
	virtual bool _update() = 0;
};

#endif