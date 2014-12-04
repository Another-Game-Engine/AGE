#include "Thread.hpp"
#include "ThreadManager.hpp"

#include <thread>

namespace AGE
{
	Thread::Thread(Thread::ThreadType type)
		: _id(type)
	{

	}

	Thread::~Thread()
	{

	}

	std::size_t Thread::_registerId()
	{
		const size_t hash = std::this_thread::get_id().hash();
		Singleton<ThreadManager>::getInstance()->registerThreadId(hash, _id);
		return _id;
	}

	std::string Thread::threadTypeToString(ThreadType t)
	{
		std::string res;
		switch (t)
		{
		case AGE::Thread::Main:
			res = "MainThread";
			break;
		case AGE::Thread::PrepareRender:
			res = "AGE_PrepareRenderThread";
			break;
		case AGE::Thread::Render:
			res = "AGE_RenderThread";
			break;
		case AGE::Thread::Worker1:
			res = "AGE_WorkerThread1";
			break;
		case AGE::Thread::Worker2:
			res = "AGE_WorkerThread2";
			break;
		case AGE::Thread::Worker3:
			res = "AGE_WorkerThread3";
			break;
		case AGE::Thread::Worker4:
			res = "AGE_WorkerThread4";
			break;
		case AGE::Thread::Worker5:
			res = "AGE_WorkerThread5";
			break;
		case AGE::Thread::END:
			assert(false); // Illegal thread type
			break;
		default:
			assert(false); // Illegal thread type
			break;
		}
		return res;
	}
}