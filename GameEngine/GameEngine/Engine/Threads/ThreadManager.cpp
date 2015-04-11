#include "ThreadManager.hpp"
#include "MainThread.hpp"
#include "PrepareRenderThread.hpp"
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
		auto pt = new PrepareRenderThread();
		auto rt = new RenderThread();
		_threads[Thread::Main] = mt;
		_threads[Thread::PrepareRender] = pt;
		_threads[Thread::Render] = rt;
		mt->linkToNext(pt);
		pt->linkToNext(rt);
		for (std::size_t i = Thread::Worker1; i < Thread::hardwareConcurency(); ++i)
		{
			_threads[i] = new TaskThread(Thread::ThreadType(i));
		}

		for (std::size_t i = Thread::Main; i < Thread::hardwareConcurency(); ++i)
		{
			_threadsStatistics[i].name = Thread::threadTypeToString(Thread::ThreadType(i));
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
	PrepareRenderThread *ThreadManager::getPrepareThread() const
	{
		return static_cast<PrepareRenderThread*>(_threads[Thread::PrepareRender]);
	}

	void ThreadManager::updateThreadStatistics(Thread::ThreadType id, std::size_t workTime, std::size_t sleepTime)
	{
		auto &s = _threadsStatistics[id];
		if (s.frameCounter >= s.work.size())
		{
			s.frameCounter = 0;
		}
		s.work[s.frameCounter] = workTime;
		s.wait[s.frameCounter++] = sleepTime;
		float newWaitTime = 0;
		float newWorkTime = 0;
		for (auto i = 0; i < s.work.size(); ++i)
		{
			newWorkTime += s.work[i];
			newWaitTime += s.wait[i];
		}
		s.averageWaitTime = newWaitTime / (float)s.work.size() / 1000.0f;
		s.averageWorkTime = newWorkTime / (float)s.work.size() / 1000.0f;
	}

	TMQ::HybridQueue *ThreadManager::getAvailableTaskQueue(bool futur, Thread::ThreadType type)
	{
		std::size_t res = -1;
		{
			std::lock_guard<SpinLock> lock(_mutex);
			if (_iterator >= Thread::hardwareConcurency())
				_iterator = Thread::Main;
			while (!_threads[_iterator]->isWorker())
			{
				++_iterator;
				if (_iterator >= Thread::hardwareConcurency())
					_iterator = Thread::Main;
			}
			res = _iterator;
			++_iterator;
		}
		_threads[res]->taskCounter++;
		// Disgusting !!!!! Heheh ! Shame on me :D
		if (res > Thread::Render)
			return static_cast<TaskThread*>(_threads[res])->getQueue();
		else if (res == 0)
			return static_cast<MainThread*>(_threads[res])->getQueue();
		else if (res == 1)
			return static_cast<PrepareRenderThread*>(_threads[res])->getQueue();
		else
			return static_cast<RenderThread*>(_threads[res])->getQueue();
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
		_threads[Thread::PrepareRender]->setAsWorker(prepareThread);
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

	PrepareRenderThread *GetPrepareThread()
	{
		return Singleton<ThreadManager>::getInstance()->getPrepareThread();
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