#pragma once

#include "Thread.hpp"
#include "QueuePusher.hpp"
#include "QueueOwner.hpp"
#include <Utils/Containers/Vector.hpp>
#include <memory>

namespace AGE
{
	class Engine;
	class AScene;

	class MainThread : public Thread, public QueuePusher, public QueueOwner
	{
	public:
		virtual bool init();
		virtual bool release();
		virtual bool launch();
		virtual bool stop();
		bool run();
		bool update();
		AGE::Engine *createEngine();
		AGE::Engine *getEngine();
		void setSceneAsActive(AScene *scene);
		inline AScene *getActiveScene() { return _activeScene; }
	private:
		MainThread();
		virtual ~MainThread();
		MainThread(const MainThread &) = delete;
		MainThread(MainThread &&) = delete;
		MainThread &operator=(const MainThread &) = delete;
		MainThread &operator=(MainThread &&) = delete;
		std::atomic_bool _insideRun;
		bool _run;

		friend class ThreadManager;

		AGE::Engine *_engine;
		AScene *_activeScene;
		TMQ::PtrQueue taskQueue;
	};
}