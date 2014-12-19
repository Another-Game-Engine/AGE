#include "ThreadManager.hpp"
#include "MainThread.hpp"
#include "PrepareRenderThread.hpp"
#include "RenderThread.hpp"
#include "TaskThread.hpp"
#include "QueueOwner.hpp"
#include "QueuePusher.hpp"

namespace AGE
{
	ThreadManager::ThreadManager()
	{
		auto hardwareConcurency = std::thread::hardware_concurrency();
		// For old computer, we need at least 3 threads hahahahaha
		// Yeah, we're like this :D
		assert(hardwareConcurency >= 3);
		_threads.resize(hardwareConcurency, nullptr);
		_threadIdReference.resize(hardwareConcurency, -1);
		auto mt = new MainThread();
		auto pt = new PrepareRenderThread();
		auto rt = new RenderThread();
		_threads[Thread::Main] = mt;
		_threads[Thread::PrepareRender] = pt;
		_threads[Thread::Render] = rt;
		mt->linkToNext(pt);
		pt->linkToNext(rt);
		for (std::size_t i = Thread::Worker1; i < hardwareConcurency; ++i)
		{
			_threads[i] = new TaskThread(Thread::ThreadType(i));
		}
	}
	
	bool ThreadManager::initAndLaunch()
	{
		bool res = true;
		for (auto &t : _threads)
		{
			res = t->launch();
			if (!res)
				return false;
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

		/*bool res = true;
		for (auto &t : _threads)
		{
			res = t->stop();
		}
		for (auto &t : _threads)
		{
			if (t)
			{
				delete t;
				t = nullptr;
			}
		}*/
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
			assert(false);
			return nullptr;
		}
	}

	std::weak_ptr<AGE::Engine> ThreadManager::createEngine()
	{
		return getMainThread()->createEngine();
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

	PrepareRenderThread *GetPrepareThread()
	{
		return Singleton<ThreadManager>::getInstance()->getPrepareThread();
	}

	std::weak_ptr<Engine> CreateEngine()
	{
		return Singleton<ThreadManager>::getInstance()->createEngine();
	}

	bool InitAGE()
	{
		static std::once_flag onceFlag;
		bool res = true;
		std::call_once(onceFlag, [&](){
			Singleton<ThreadManager>::setInstance();
			auto threadManager = Singleton<ThreadManager>::getInstance();
			res = threadManager->initAndLaunch();
			if (!res)
				return;
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