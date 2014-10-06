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

	_commandQueue.getDispatcher()
		.handle<RendCtxCommand::Flush>([&](const RendCtxCommand::Flush& msg)
	{
		_context->swapContext();
	})
		.handle<RendCtxCommand::GetScreenSize>([&](RendCtxCommand::GetScreenSize& msg)
	{
		msg.result.set_value(_context->getScreenSize());
	})
		.handle<RendCtxCommand::SetScreenSize>([&](const RendCtxCommand::SetScreenSize& msg)
	{
		_context->setScreenSize(msg.screenSize);
	})
		.handle<AGE::TQC::BoolFunction>([&](AGE::TQC::BoolFunction& msg)
	{
		msg.result.set_value(msg.function());
	})
		.handle<RendCtxCommand::RefreshInputs>([&](const RendCtxCommand::RefreshInputs& msg)
	{
		_context->refreshInputs();
	})
		.handle<TQC::VoidFunction>([&](const TQC::VoidFunction& msg)
	{
		if (msg.function)
			msg.function();
	})
#ifdef USE_IMGUI
		.handle<RendCtxCommand::RenderImgui>([&](const RendCtxCommand::RenderImgui &msg)
	{
		AGE::Imgui::getInstance()->renderThreadRenderFn(msg.cmd_lists, msg.cmd_lists_count);
	})
#endif
		.handle<TMQ::CloseQueue>([&](TMQ::CloseQueue& msg)
	{
		return false;
	});

	return returnValue;
}

RenderThread::RenderThread()
{}

RenderThread::~RenderThread()
{}