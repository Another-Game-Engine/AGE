#pragma once

#include "Thread.hpp"
#include "CommandQueuePusher.hpp"
#include "CommandQueueOwner.hpp"
#include "TaskQueueOwner.hpp"

#include <Utils/Containers/Vector.hpp>
#include <memory>


namespace AGE
{
	class RenderScene{};

	class PrepareRenderThread : public Thread, public CommandQueuePusher, public CommandQueueOwner, public TaskQueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
	private:
		PrepareRenderThread();
		virtual ~PrepareRenderThread();
		PrepareRenderThread(const PrepareRenderThread &) = delete;
		PrepareRenderThread(PrepareRenderThread &&) = delete;
		PrepareRenderThread &operator=(const PrepareRenderThread &) = delete;
		PrepareRenderThread &operator=(PrepareRenderThread &&) = delete;

		void _createRenderScene(){} // todo -> create a scene when scene created in Main thread
		void _destroyRenderScene(AGE::RenderScene *scene){}; // todo -> delete a scene when scene deleted in Main thread

		AGE::Vector < std::unique_ptr<AGE::RenderScene> > _scenes;
		AGE::RenderScene *_activeScene;

		friend class ThreadManager;
	};
}