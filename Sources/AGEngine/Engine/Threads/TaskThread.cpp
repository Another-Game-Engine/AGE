#include "TaskThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/ThreadManager.hpp>

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
		registerCallback<Tasks::Basic::VoidFunction>([&](Tasks::Basic::VoidFunction &msg){
			if (msg.function)
				msg.function();
		});

		registerCallback<AGE::Tasks::Basic::Exit>([&](AGE::Tasks::Basic::Exit& msg)
		{
			this->_insideRun = false;
			TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::Exit>();
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
#ifdef AGE_ENABLE_PROFILING
		MicroProfileOnThreadExit();
#endif
		return true;
	}

	bool TaskThread::stop()
	{
		TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::Exit>();
		if (_threadHandle.joinable())
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

		TMQ::MessageBase *task = nullptr;
		while (_run && _insideRun)
		{
			while (TMQ::TaskManager::TaskThreadGetTask(task) == false)
			{ }
			if (task != nullptr)
			{
				//pop all tasks
				auto result = execute(task);
				assert(result); // we receive a task that we cannot treat
				taskCounter--;
			}
		}
		return true;
	}
}