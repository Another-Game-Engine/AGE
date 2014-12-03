#include "RenderThread.hpp"
#include <Utils/ThreadQueueCommands.hpp>

#include <Configuration.hpp>

#include <Utils/Age_Imgui.hpp>
#include <chrono>

#ifdef USE_IMGUI
#include <imgui/imgui.h>
#endif

#include <Core/Engine.hh>

using namespace AGE;

bool OldRenderThread::_init()
{
	_context = _engine->setInstance<SdlContext, IRenderContext>();
	_renderManager = _engine->setInstance<gl::RenderManager>();


	registerMessageCallback<RendCtxCommand::Flush>([&](RendCtxCommand::Flush& msg)
	{
		_context->swapContext();
	});

	registerMessageCallback<RendCtxCommand::GetScreenSize>([&](RendCtxCommand::GetScreenSize& msg)
	{
		msg.setValue(_context->getScreenSize());
	});

	registerMessageCallback<RendCtxCommand::SetScreenSize>([&](RendCtxCommand::SetScreenSize& msg)
	{
		_context->setScreenSize(msg.screenSize);
	});

	registerMessageCallback<AGE::TQC::BoolFunction>([&](AGE::TQC::BoolFunction& msg)
	{
		msg.setValue(msg.function());
	});

	registerMessageCallback<RendCtxCommand::RefreshInputs>([&](RendCtxCommand::RefreshInputs& msg)
	{
		_context->refreshInputs();
	});

	registerMessageCallback<RendCtxCommand::CopyDrawLists>([&](RendCtxCommand::CopyDrawLists& msg)
	{
		this->_toDrawList = std::move(msg.list);
	});

	registerMessageCallback<RendCtxCommand::RenderDrawLists>([&](RendCtxCommand::RenderDrawLists& msg)
	{
		for (auto &e : this->_toDrawList)
		{
			msg.function(e);
		}
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
	return true;
}

bool OldRenderThread::_initInNewThread()
{
	return true;
}

bool OldRenderThread::_release()
{
	return true;
}

bool OldRenderThread::_releaseInNewThread()
{
	_engine->deleteInstance<IRenderContext>();
	_engine->deleteInstance<gl::RenderManager>();
	return true;
}

bool OldRenderThread::_updateBegin()
{
	bool returnValue = true;
	return returnValue;
}

bool OldRenderThread::_updateEnd()
{
	bool returnValue = true;
	_next->getTaskQueue()->emplace<AGE::MTC::FrameTime>(this->_threadId, std::chrono::duration_cast<std::chrono::milliseconds>(_elapsed).count());
	return returnValue;
}


OldRenderThread::OldRenderThread()
{}

OldRenderThread::~OldRenderThread()
{}