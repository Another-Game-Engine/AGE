#pragma once

#include "Thread.hpp"
#include <Utils/Singleton.hh>
#include <Utils/Containers/Vector.hpp>
//to remove
#include <thread>

namespace AGE
{
	class MainThread;
	class PrepareRenderThread;
	class RenderThread;

	class ThreadManager
	{
	public:
		Thread *getCurrentThread() const;
		MainThread *getMainThread() const;
		RenderThread *getRenderThread() const;
		PrepareRenderThread *getPrepareThread() const;
	private:
		ThreadManager();
		virtual ~ThreadManager();
		void registerThreadId(std::size_t systemHash, Thread::ThreadType type);

		AGE::Vector<AGE::Thread*> _threads;
		AGE::Vector<std::size_t>  _threadIdReference;

		friend class Singleton < ThreadManager >;
		friend class Thread;
	};

	static Thread *currentThread();
	static MainThread *getMainThread();
	static RenderThread *getRenderThread();
	static PrepareRenderThread *getPrepareThread();
}