#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include <Utils/Containers/Vector.hpp>
#include <memory>

namespace AGE
{
	class Engine;

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
		std::atomic_bool _run;

		friend class ThreadManager;
	};

	/***********************MAIN THREAD -> RENDER THREAD MESSAGES ********************/
	struct MainThreadToRenderThread
	{
	private:
		struct CreateRenderContext : TMQ::FutureData<bool>
		{
			CreateRenderContext(std::weak_ptr<Engine> _engine) : engine(_engine){}
			std::weak_ptr<Engine> engine;
		};
		friend class RenderThread;
		friend class MainThread;
	};

	/*********************************************************************************/

}