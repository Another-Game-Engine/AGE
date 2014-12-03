#include "CommandQueueOwner.hpp"

namespace AGE
{
	AGE::Vector<std::unique_ptr<ICallbackContainer>> CommandQueueOwner::_sharedCommandCallbacks = AGE::Vector<std::unique_ptr<ICallbackContainer>>();
}