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
		virtual bool launch();
		virtual bool stop();
		bool run();
		bool update();
		std::weak_ptr<AGE::Engine> createEngine();
		std::weak_ptr<AGE::Engine> getEngine();
	private:
		MainThread();
		virtual ~MainThread();
		MainThread(const MainThread &) = delete;
		MainThread(MainThread &&) = delete;
		MainThread &operator=(const MainThread &) = delete;
		MainThread &operator=(MainThread &&) = delete;
		std::atomic_bool _run;

		friend class ThreadManager;

		std::shared_ptr<AGE::Engine> _engine;
	};
}