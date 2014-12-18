#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include <Utils/Containers/Vector.hpp>
#include <memory>

class SdlContext;
class Input;
namespace gl
{
	class RenderManager;
}
namespace AGE
{
	class Engine;
	struct DrawableCollection;

	class RenderThread : public Thread, public QueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
		virtual bool launch();
		virtual bool stop();
	private:
		RenderThread();
		virtual ~RenderThread();
		RenderThread(const RenderThread &) = delete;
		RenderThread(RenderThread &&) = delete;
		RenderThread &operator=(const RenderThread &) = delete;
		RenderThread &operator=(RenderThread &&) = delete;

		std::thread _threadHandle;
		std::atomic_bool _insideRun;
		bool _run;

		SdlContext *_context;
		gl::RenderManager *_render;
		AGE::Vector < AGE::DrawableCollection > _drawlist;

		friend class ThreadManager;
	};
}