#include "CallbackContainer.hpp"

namespace AGE
{
	ICallbackContainer::~ICallbackContainer()
	{}

	ICallbackContainer::ICallbackContainer()
		: _isValid(false)
	{}
}