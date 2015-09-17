#pragma once

#include "FSThreadBase.hpp"

namespace AGE
{
	FSThreadBase::~FSThreadBase(void)
	{
		resume();
		stop();
	}

	void FSThreadBase::run(void)
	{
		if (!running)
		{
			running = true;
			thread = std::thread([this](void)
			{
				FSThreadBase::Handler(this);
			});
		}
	}

	void FSThreadBase::stop(void)
	{
		if (running)
		{
			running = false;
			resume();
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

	void FSThreadBase::resume(void)
	{
		if (waiting)
		{
			std::unique_lock<std::mutex> lock(mutex);
			waiting = false;
			condition.notify_all();
		}
	}

	void FSThreadBase::pause(void)
	{
		if (running && !waiting)
		{
			std::unique_lock<std::mutex> lock(mutex);
			waiting = true;
			while (waiting)
			{
				condition.wait(lock);
			}
		}
	}

	bool FSThreadBase::isRunning(void) const
	{
		return running;
	}

	bool FSThreadBase::isWaiting(void) const
	{
		return waiting;
	}

	void FSThreadBase::lock(void)
	{
		locker.lock();
	}

	void FSThreadBase::unlock(void)
	{
		locker.unlock();
	}

	bool FSThreadBase::isLocked(void) const
	{
		return locker.isLocked();
	}

	void FSThreadBase::Handler(FSThreadBase *thread)
	{
		if (thread->running)
		{
			thread->process();
			thread->running = false;
		}
	}
}
