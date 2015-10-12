#include "ThreadManager.hpp"
#include "MainThread.hpp"
#include "RenderThread.hpp"
#include "TaskThread.hpp"
#include "QueueOwner.hpp"
#include "QueuePusher.hpp"
#include <TMQ/queue.hpp>
#include <limits>
#include <Utils/Debug.hpp>

namespace AGE
{
	ThreadManager::ThreadManager()
		: _engine(nullptr)
	{
		// For old computer, we need at least 3 threads hahahahaha
		// Yeah, we're like this :D
		assert(Thread::hardwareConcurency() >= 3);
		_threads.resize(Thread::hardwareConcurency(), nullptr);
		_threadIdReference.resize(Thread::hardwareConcurency(), -1);
		auto mt = new MainThread();
		auto rt = new RenderThread();

		_threads[Thread::Main] = mt;
		_threads[Thread::Render] = rt;

		for (std::size_t i = Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
		{
			_threads[i] = new TaskThread(Thread::ThreadType(i));
		}
	}
	
	bool ThreadManager::initAndLaunch()
	{
		int i = 0;
		for (auto &t : _threads)
		{
			bool res = t->launch();
			AGE_ASSERT(res);
			if (i > Thread::Render)
			{
				auto voidHandle = t->getThreadHandle().native_handle();
				HANDLE handle = (HANDLE)(voidHandle);
				//auto success = SetThreadAffinityMask(handle, 1 << i);
				auto success = SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
				AGE_ASSERT(success != 0);
			}
			++i;
		}
		{
			HANDLE handle = ::GetCurrentThread();
			auto success = SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
			//auto success = SetThreadAffinityMask(handle, 0x1);
			AGE_ASSERT(success != 0);
		}
		{
			auto voidHandle = GetRenderThread()->getThreadHandle().native_handle();
			HANDLE handle = (HANDLE)(voidHandle);
			auto success = SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
			//auto success = SetThreadAffinityMask(handle, 0x2);
			AGE_ASSERT(success != 0);
		}
		return true;
	}

	void ThreadManager::exit()
	{
		auto hardwareConcurency = std::thread::hardware_concurrency();
		for (std::size_t i = Thread::Worker1; i < hardwareConcurency; ++i)
		{
			_threads[i]->stop();
			delete _threads[i];
			_threads[i] = nullptr;
		}
		_threads[Thread::Main]->stop();
		_threads[Thread::Render]->stop();

		_threads.clear();
	}

	ThreadManager::~ThreadManager()
	{
		for (auto &e : _threads)
		{
			e->stop();
			delete e;
		}
	}

	void ThreadManager::registerThreadId(std::size_t systemHash, Thread::ThreadType type)
	{
		assert(type < _threadIdReference.size());
		_threadIdReference[type] = systemHash;
	}

	Thread *ThreadManager::getCurrentThread() const
	{
		auto hash = std::this_thread::get_id().hash();
		std::size_t id = -1;
		for (std::size_t i = 0; i < _threadIdReference.size(); ++i)
		{
			id = _threadIdReference[i];
			if (id == hash)
			{
				assert(_threads[i] != nullptr);
				return _threads[i];
			}
		}
		assert(false);
		return nullptr;
	}

	Engine *ThreadManager::createEngine()
	{
		_engine = getMainThread()->createEngine();
		return _engine;
	}

	AGE::Engine *ThreadManager::getEngine()
	{
		AGE_ASSERT(_engine != nullptr);
		return _engine;
	}

	MainThread *ThreadManager::getMainThread() const
	{
		return static_cast<MainThread*>(_threads[Thread::Main]);
	}

	RenderThread *ThreadManager::getRenderThread() const
	{
		return static_cast<RenderThread*>(_threads[Thread::Render]);
	}

	void ThreadManager::forEachThreads(std::function<void(AGE::Thread *)> &&fn)
	{
		for (auto &e : _threads)
		{
			fn(e);
		}
	}

	void ThreadManager::setAsWorker(bool mainThread, bool prepareThread, bool renderThread)
	{
		_threads[Thread::Main]->setAsWorker(mainThread);
		_threads[Thread::Render]->setAsWorker(renderThread);
	}

	ThreadManager *GetThreadManager()
	{
		return Singleton<ThreadManager>::getInstance();
	}

	Thread *CurrentThread()
	{
		return Singleton<ThreadManager>::getInstance()->getCurrentThread();
	}

	MainThread *GetMainThread()
	{
		return Singleton<ThreadManager>::getInstance()->getMainThread();
	}

	RenderThread *GetRenderThread()
	{
		return Singleton<ThreadManager>::getInstance()->getRenderThread();
	}

	bool InitAGE()
	{
		static std::once_flag onceFlag;
		bool res = true;
		std::call_once(onceFlag, [&](){
			Singleton<ThreadManager>::setInstance();
			Singleton<AGE::Logger>::setInstance();
			auto threadManager = Singleton<ThreadManager>::getInstance();
			res = threadManager->initAndLaunch();
			return res;
		});
		return res;
	}

	void ExitAGE()
	{
		static std::once_flag onceFlag;
		std::call_once(onceFlag, [&](){
			auto threadManager = Singleton<ThreadManager>::getInstance();
			threadManager->exit();
		});
	}
}