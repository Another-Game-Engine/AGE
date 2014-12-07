#pragma once

#include <Utils/ThreadQueue.hpp>
#include <Context/SdlContext.hh>
#include <Render/RenderManager.hh>
#include <Utils/Dependency.hpp>
#include <Context/IRenderContext.hh>
#include <Render/RenderThreadInterface.hpp>

////
// Default render thread, you can also build yours

namespace AGE
{
	class RenderThread : public RenderThreadInterface
	{
		SdlContext *_context;

		virtual bool _init();
		virtual bool _initInNewThread();
		virtual bool _release();
		virtual bool _releaseInNewThread();
		virtual bool _update();
	public:
		RenderThread();
		virtual ~RenderThread();
	};
}