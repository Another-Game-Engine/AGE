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




OldRenderThread::OldRenderThread()
{}

OldRenderThread::~OldRenderThread()
{}