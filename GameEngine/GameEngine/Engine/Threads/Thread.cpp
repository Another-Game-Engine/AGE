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
}