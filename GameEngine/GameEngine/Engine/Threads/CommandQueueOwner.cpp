#include "CommandQueueOwner.hpp"

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
		return true;
	}
}