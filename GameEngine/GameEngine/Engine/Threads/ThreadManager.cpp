#include "ThreadManager.hpp"

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
					assert(_threads[id] != nullptr);
					return _threads[i];
				}
				assert(false);
				return nullptr;
			}
		}
}