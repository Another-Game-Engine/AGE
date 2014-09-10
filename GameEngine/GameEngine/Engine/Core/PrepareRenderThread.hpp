#pragma once

#include "PrepareRenderThreadInterface.hpp"

namespace AGE
{
	class PrepareRenderThread : public PrepareRenderThreadInterface
	{
		virtual bool _init();
		virtual bool _initInNewThread();
		virtual bool _release();
		virtual bool _releaseInNewThread();
		virtual bool _update();
	public:
		PrepareRenderThread();
		virtual ~PrepareRenderThread();
	};
}
