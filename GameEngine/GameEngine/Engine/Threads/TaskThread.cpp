#include "TaskThread.hpp"
#include <Utils/ThreadName.hpp>
#include <Core/Tasks/Basics.hpp>
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

		std::chrono::system_clock::time_point waitStart;
		std::chrono::system_clock::time_point waitEnd;
		std::chrono::system_clock::time_point workStart;
		std::chrono::system_clock::time_point workEnd;

		getQueue()->setWaitingTime(100);
		TMQ::PtrQueue tasks;
		while (_run && _insideRun)
		{
			waitStart = std::chrono::high_resolution_clock::now();
			getQueue()->getTaskQueue(tasks, TMQ::HybridQueue::Wait);
			waitEnd = std::chrono::high_resolution_clock::now();
			workStart = std::chrono::high_resolution_clock::now();
			while (!tasks.empty())
			{
				//pop all tasks
				auto task = tasks.front();
				assert(execute(task)); // we receive a task that we cannot treat
				tasks.pop();
			}
			workEnd = std::chrono::high_resolution_clock::now();
			GetThreadManager()->updateThreadStatistics(this->_id
				, std::chrono::duration_cast<std::chrono::microseconds>(workEnd - workStart).count()
				, std::chrono::duration_cast<std::chrono::microseconds>(waitEnd - waitStart).count());
		}
		return true;
	}
}