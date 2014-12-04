#pragma once

#include "Thread.hpp"
#include "CommandQueuePusher.hpp"
#include "TaskQueueOwner.hpp"

#include <Utils/Containers/Vector.hpp>
#include <memory>


namespace AGE
{
	class Engine;

	class MainThread : public Thread, public CommandQueuePusher, public TaskQueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		bool update();
		AGE::Engine *createEngine();
		void destroyEngine(AGE::Engine *engine);
	private:
		MainThread();
		virtual ~MainThread();
		MainThread(const MainThread &) = delete;
		MainThread(MainThread &&) = delete;
		MainThread &operator=(const MainThread &) = delete;
		MainThread &operator=(MainThread &&) = delete;

		friend class ThreadManager;

		AGE::Vector < std::unique_ptr<AGE::Engine> > _engines;
		AGE::Engine *_activeEngine;
	};
}