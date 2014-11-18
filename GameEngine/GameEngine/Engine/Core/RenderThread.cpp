#include "RenderThread.hpp"
#include <Utils/ThreadQueueCommands.hpp>

#include <Configuration.hpp>

#include <Utils/Age_Imgui.hpp>
#include <chrono>

#ifdef USE_IMGUI
#include <imgui/imgui.h>
#endif

using namespace AGE;

bool RenderThread::_init()
{
	_context = _engine->setInstance<SdlContext, IRenderContext>();
	_renderManager = _engine->setInstance<gl::RenderManager>();

	registerMessageCallback<RendCtxCommand::Flush>([&](RendCtxCommand::Flush& msg)
	{
		_context->swapContext();
	});

	registerMessageCallback<RendCtxCommand::GetScreenSize>([&](RendCtxCommand::GetScreenSize& msg)
	{
		msg.result.set_value(_context->getScreenSize());
	});

	registerMessageCallback<RendCtxCommand::SetScreenSize>([&](RendCtxCommand::SetScreenSize& msg)
	{
		_context->setScreenSize(msg.screenSize);
	});

	registerMessageCallback<AGE::TQC::BoolFunction>([&](AGE::TQC::BoolFunction& msg)
	{
		msg.result.set_value(msg.function());
	});

	registerMessageCallback<RendCtxCommand::RefreshInputs>([&](RendCtxCommand::RefreshInputs& msg)
	{
		_context->refreshInputs();
	});

	registerMessageCallback<TQC::VoidFunction>([&](TQC::VoidFunction& msg)
	{
		if (msg.function)
			msg.function();
	});

#ifdef USE_IMGUI
	registerMessageCallback<AGE::RenderImgui>([&](AGE::RenderImgui &msg)
	{
		AGE::Imgui::getInstance()->renderThreadRenderFn(msg.cmd_lists);
	});
#endif

	registerMessageCallback<TMQ::CloseQueue>([&](TMQ::CloseQueue& msg)
	{
		std::cout << "close";
	});

	return true;
}

bool RenderThread::_initInNewThread()
{
	return true;
}

bool RenderThread::_release()
{
	return true;
}

bool RenderThread::_releaseInNewThread()
{
	_engine->deleteInstance<IRenderContext>();
	_engine->deleteInstance<gl::RenderManager>();
	return true;
}

bool RenderThread::_update()
{
	bool returnValue = true;

	

	return returnValue;
}

RenderThread::RenderThread()
{}

RenderThread::~RenderThread()
{}