#include "TaskQueueOwner.hpp"

namespace AGE
{
	AGE::Vector<std::unique_ptr<ICallbackContainer>> TaskQueueOwner::_sharedTaskCallbacks = AGE::Vector<std::unique_ptr<ICallbackContainer>>();

	bool TaskQueueOwner::executeTask(TMQ::MessageBase *task)
	{
		std::size_t id = task->uid;
		if (_individualTaskCallbacks.size() > id && _individualTaskCallbacks[id] != nullptr)
		{
			(*_individualTaskCallbacks[id].get())(task);
		}
		else if (_sharedTaskCallbacks.size() > id && _sharedTaskCallbacks[id] != nullptr)
		{
			(*_sharedTaskCallbacks[id].get())(task);
		}
		else
		{
			return false;
		}
		task->_used = true;
		return true;
	}
}