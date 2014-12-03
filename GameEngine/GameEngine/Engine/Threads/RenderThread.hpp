#pragma once

#include "Thread.hpp"
#include "CommandQueuePusher.hpp"
#include "CommandQueueOwner.hpp"
#include "TaskQueueOwner.hpp"

#include <Utils/Containers/Vector.hpp>
#include <memory>

namespace AGE
{
	class RenderContext;

	class RenderThread : public Thread, public CommandQueuePusher, public CommandQueueOwner, public TaskQueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
	private:
		RenderThread();
		virtual ~RenderThread();
		RenderThread(const RenderThread &) = delete;
		RenderThread(RenderThread &&) = delete;
		RenderThread &operator=(const RenderThread &) = delete;
		RenderThread &operator=(RenderThread &&) = delete;

		void _createRenderContext(){} // todo -> create a render context when an engine is created in Main thread
		void _destroyRenderContext(AGE::RenderContext *context){}; // todo -> delete a render context when an engine deleted in Main thread

		AGE::Vector < std::unique_ptr<AGE::RenderContext> > _contexts;
		AGE::RenderContext *_activeContext;
	};
}