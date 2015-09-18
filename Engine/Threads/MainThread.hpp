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
		inline AScene *getActiveScene() { return _activeScene; }
		inline bool isRenderFrame() const { return _isRenderFrame; }
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

		bool _isRenderFrame;
		std::size_t _frameCounter;

		AGE::Engine *_engine;
		AScene *_activeScene;
		TMQ::PtrQueue taskQueue;
	};
}