#include "QueueOwner.hpp"

namespace AGE
{
	AGE::Vector<std::unique_ptr<ICallbackContainer>> QueueOwner::_sharedCommandCallbacks = AGE::Vector<std::unique_ptr<ICallbackContainer>>();

	bool QueueOwner::execute(TMQ::MessageBase *task)
	{
		std::size_t id = task->uid;
		if (_individualCommandCallbacks.size() > id && _individualCommandCallbacks[id] != nullptr)
		{
			(*_individualCommandCallbacks[id].get())(task);
		}
		else if (_sharedCommandCallbacks.size() > id && _sharedCommandCallbacks[id] != nullptr)
		{
			(*_sharedCommandCallbacks[id].get())(task);
		}
		else
		{
			return false;
		}
		task->_used = true;
		delete task;
		return true;
	}

	void QueueOwner::computeTasksWhile(std::function<bool()> &condition)
	{
		SCOPE_profile_cpu_function("QueueOwner");
		TMQ::MessageBase *task = nullptr;

		while (true)
		{
			{
				SCOPE_profile_cpu_i("QueueOwner", "condition begin");
				if (condition())
				{
					return;
				}
			}
			{
				SCOPE_profile_cpu_i("QueueOwner", "try to get task");
				getQueue()->tryToGetSharedTask(task, 1);
			}
			if (task != nullptr)
			{
				SCOPE_profile_cpu_i("QueueOwner", "execute");
				auto result = execute(task);
				assert(result); // we receive a task that we cannot treat
			}
			{
				SCOPE_profile_cpu_i("QueueOwner", "condition end");
				if (condition())
				{
					return;
				}
			}
		}
	}
}