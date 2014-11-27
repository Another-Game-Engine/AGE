#include "ThreadQueue.hpp"
#include <Core/Engine.hh>

namespace AGE
{
	bool ThreadQueue::runFunction(Engine *engine)
	{
		bool run = true;
		_threadId = std::this_thread::get_id().hash();
		run = _initInNewThread();
		engine->getTaskQueue()->emplace<MTC::ThreadRegistration>(_threadId, _name);
		while (_run && run)
		{
			if (!commandQueueUpdate())
				break;
		}
		return _releaseInNewThread();
	}

	ThreadQueue::ThreadQueue()
	{}

	ThreadQueue::~ThreadQueue()
	{}

	bool ThreadQueue::launch(Engine *engine, const std::string &name)
	{
		_name = name;
		_engine = engine;
		_run = true;
		auto res = init(engine);
		_thread = std::thread(&ThreadQueue::runFunction, std::ref(*this), engine);
#ifdef WIN32
		DWORD threadId = ::GetThreadId(static_cast<HANDLE>(_thread.native_handle()));
		SetThreadName(threadId, _name.c_str());
#endif
		return res;
	}

	void ThreadQueue::quit()
	{
		if (!_thread.joinable())
			return;
		_run = false;
		_thread.join();
		_release();
	}

}