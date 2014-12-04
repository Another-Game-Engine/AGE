#include "ThreadManager.hpp"
#include "MainThread.hpp"
#include "PrepareRenderThread.hpp"
#include "RenderThread.hpp"

namespace AGE
{
	ThreadManager::ThreadManager()
	{
		auto hardwareConcurency = std::thread::hardware_concurrency();
		// To old computer, we need at least 3 threads hahahahaha
		// Yeah, we're like this :D
		assert(hardwareConcurency >= 3);
		_threads.resize(hardwareConcurency, nullptr);
		_threadIdReference.resize(hardwareConcurency, -1);
		_threads[Thread::Main] = new MainThread();
		_threads[Thread::PrepareRender] = new PrepareRenderThread();
		_threads[Thread::Render] = new RenderThread();
		for (std::size_t i = Thread::Worker1; i < hardwareConcurency; ++i)
		{
			// Todo
			//_threads[i] = new TaskThread();
		}
	}

	ThreadManager::~ThreadManager()
	{
		for (auto &e : _threads)
		{
			e->release();
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
			assert(false);
			return nullptr;
		}
	}

	MainThread *ThreadManager::getMainThread() const
	{
		return static_cast<MainThread*>(_threads[Thread::Main]);
	}

	RenderThread *ThreadManager::getRenderThread() const
	{
		return static_cast<RenderThread*>(_threads[Thread::Render]);
	}
	PrepareRenderThread *ThreadManager::getPrepareThread() const
	{
		return static_cast<PrepareRenderThread*>(_threads[Thread::PrepareRender]);
	}

	static Thread *currentThread()
	{
		return Singleton<ThreadManager>::getInstance()->getCurrentThread();
	}

	static MainThread *getMainThread()
	{
		return Singleton<ThreadManager>::getInstance()->getMainThread();
	}

	static RenderThread *getRenderThread()
	{
		return Singleton<ThreadManager>::getInstance()->getRenderThread();
	}

	static PrepareRenderThread *getPrepareThread()
	{
		return Singleton<ThreadManager>::getInstance()->getPrepareThread();
	}

}