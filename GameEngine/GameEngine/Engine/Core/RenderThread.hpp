#pragma once

#include <Context/SdlContext.hh>
#include <Render/RenderManager.hh>
#include <Utils/Dependency.hpp>
#include <Context/IRenderContext.hh>

////
// Default render thread, you can also build yours

namespace AGE
{
	class OldRenderThread
	{
		SdlContext *_context;
		gl::RenderManager *_renderManager;

		AGE::Vector<DrawableCollection> _toDrawList;

		virtual bool _init();
		virtual bool _initInNewThread();
		virtual bool _release();
		virtual bool _releaseInNewThread();
		virtual bool _updateBegin();
		virtual bool _updateEnd();
	public:
		OldRenderThread();
		virtual ~OldRenderThread();
	};
}