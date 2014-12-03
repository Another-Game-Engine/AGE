#include "TaskQueueOwner.hpp"

namespace AGE
{
	AGE::Vector<std::unique_ptr<ICallbackContainer>> TaskQueueOwner::_sharedTaskCallbacks = AGE::Vector<std::unique_ptr<ICallbackContainer>>();
}