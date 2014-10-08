#include "RenderThread.hpp"
#include <Utils/ThreadQueueCommands.hpp>

#include <Configuration.hpp>

#include <Utils/Age_Imgui.hpp>
#include <chrono>

using namespace AGE;

bool RenderThread::_init()
{
	_context = _engine->setInstance<SdlContext, IRenderContext>();
	_renderManager = _engine->setInstance<gl::RenderManager>();
	return true;
}

bool RenderThread::_initInNewThread()
{
	AGE::Imgui::getInstance()->registerThread(100);
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
	static auto frameStart = std::chrono::high_resolution_clock::now();

	_commandQueue.getDispatcher()
		.handle<RendCtxCommand::Flush>([&](const RendCtxCommand::Flush& msg)
	{
#ifdef USE_IMGUI
		IMGUI_BEGIN
		ImGui::Text("Coucou from render thread");
		ImGui::Render();
		IMGUI_END
		Imgui::getInstance()->threadLoopEnd();
		AGE::Imgui::getInstance()->endUpdate();
#endif
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
		.handle<TQC::StartOfFrame>([&](const TQC::StartOfFrame& msg)
		{
			frameStart = std::chrono::system_clock::now();
		}).handle<TQC::EndOfFrame>([&](const TQC::EndOfFrame& msg)
		{
			auto t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
			IMGUI_BEGIN
				ImGui::Text("Render Thread : %i ms", t.count());
			IMGUI_END
		})
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