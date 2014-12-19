#include "TaskThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Core/Tasks/Basics.hpp>

namespace AGE
{
	TaskThread::TaskThread(Thread::ThreadType type)
		: Thread(type)
	{
		assert(type >= Thread::ThreadType::Worker1 && type < Thread::ThreadType::END);
	}

	TaskThread::~TaskThread()
	{}

	bool TaskThread::init()
	{
		registerCallback<Tasks::Basic::VoidFunction>([this](Tasks::Basic::VoidFunction &msg){
			if (msg.function)
				msg.function();
		});
		return true;
	}

	bool TaskThread::release()
	{
		return true;
	}

	bool TaskThread::launch()
	{
		if (!init())
			return false;
		_threadHandle = std::thread(&TaskThread::update, std::ref(*this));
		return true;
	}

	bool TaskThread::stop()
	{
		_insideRun = false;
		getQueue()->emplaceTask<Tasks::Basic::VoidFunction>([](){});
		_threadHandle.join();
		return true;
	}

	bool TaskThread::update()
	{
		_registerId();
		_run = true;
		_insideRun = true;
		DWORD threadId = ::GetThreadId(static_cast<HANDLE>(_threadHandle.native_handle()));
		SetThreadName(threadId, _name.c_str());
		while (_run && _insideRun)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		return true;
	}
}