#pragma once

#include "Thread.hpp"
#include <Utils/Singleton.hh>
#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <atomic>
#include <array>

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
		void exit();
		void updateThreadStatistics(Thread::ThreadType type, std::size_t workTime, std::size_t waitTime);
		struct ThreadStatistics
		{
			std::string name;
			float averageWorkTime;
			float averageWaitTime;
			float averageWorkTimeCopy;
			float averageWaitTimeCopy;
			std::atomic_size_t frameCounter;
			std::mutex mutex;
			std::array < std::size_t, 64 > work;
			std::array < std::size_t, 64 > wait;
			std::array < float, 64 > workCopy;
			std::array < float , 64 > waitCopy;
			std::array < float , 64 > totalCopy;
			ThreadStatistics()
				: name("")
				, frameCounter(0)
			{
				work.fill(0);
				wait.fill(0);
				averageWorkTime = 0;
				averageWaitTime = 0;
			}
		};
		inline std::array<ThreadStatistics, Thread::END> &getStatistics()
		{
			return _threadsStatistics;
		}
	private:
		std::array<ThreadStatistics, Thread::END> _threadsStatistics;

		ThreadManager();
		virtual ~ThreadManager();
		ThreadManager(const ThreadManager &o) = delete;
		ThreadManager(ThreadManager &&o) = delete;
		ThreadManager &operator=(const ThreadManager &o) = delete;
		ThreadManager &operator=(ThreadManager &&o) = delete;
		void registerThreadId(std::size_t systemHash, Thread::ThreadType type);

		AGE::Vector<AGE::Thread*> _threads;
		AGE::Vector<std::size_t>  _threadIdReference;

		friend class Singleton < ThreadManager >;
		friend class Thread;
	};

	ThreadManager *GetThreadManager();
	Thread *CurrentThread();
	MainThread *GetMainThread();
	RenderThread *GetRenderThread();
	PrepareRenderThread *GetPrepareThread();
	std::weak_ptr<Engine> CreateEngine();
	bool InitAGE();
	void ExitAGE();
}