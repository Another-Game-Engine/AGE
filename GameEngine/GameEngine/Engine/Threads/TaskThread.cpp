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

		registerCallback<AGE::Tasks::Basic::Exit>([&](AGE::Tasks::Basic::Exit& msg)
		{
			this->_insideRun = false;
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
		getQueue()->emplaceTask<Tasks::Basic::Exit>();
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

		TMQ::PtrQueue tasks;
		while (_run && _insideRun)
		{
			if (getQueue()->getTaskQueue(tasks, TMQ::HybridQueue::Block))
			{
				while (!tasks.empty())
				{
					//pop all tasks
					auto task = tasks.front();
					assert(execute(task)); // we receive a task that we cannot treat
					tasks.pop();
				}
			}
		}
		return true;
	}
}