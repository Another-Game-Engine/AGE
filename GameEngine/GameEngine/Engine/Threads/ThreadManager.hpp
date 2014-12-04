#pragma once

#include "Thread.hpp"
#include <Utils/Singleton.hh>
#include <Utils/Containers/Vector.hpp>
#include <memory>

namespace AGE
{
	class MainThread;
	class PrepareRenderThread;
	class RenderThread;
	class Engine;

	class ThreadManager
	{
	public:
		Thread *getCurrentThread() const;
		MainThread *getMainThread() const;
		RenderThread *getRenderThread() const;
		PrepareRenderThread *getPrepareThread() const;
		std::weak_ptr<AGE::Engine> createEngine();
		bool initAndLaunch();
	private:
		ThreadManager();
		virtual ~ThreadManager();
		void registerThreadId(std::size_t systemHash, Thread::ThreadType type);

		AGE::Vector<AGE::Thread*> _threads;
		AGE::Vector<std::size_t>  _threadIdReference;

		friend class Singleton < ThreadManager >;
		friend class Thread;
	};

	Thread *CurrentThread();
	MainThread *GetMainThread();
	RenderThread *GetRenderThread();
	PrepareRenderThread *GetPrepareThread();
	std::weak_ptr<Engine> CreateEngine();
	bool InitAGE();
}