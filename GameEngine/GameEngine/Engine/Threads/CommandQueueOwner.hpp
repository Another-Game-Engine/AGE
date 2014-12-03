#pragma once

#include <TMQ/queue.hpp>
#include "CallbackContainer.hpp"
#include <Utils/Containers/Vector.hpp>

namespace AGE
{
	class CommandQueuePusher;

	class CommandQueueOwner
	{
	public:
		inline TMQ::ReleasableQueue *getCommandQueue() { return &_queue; }

		template <typename T>
		void registerCommandCallback(const std::function<void(T &)> &fn)
		{
			auto id = TMQ::Message<T>::getId();
			if (id >= _individualCommandCallbacks.size())
				_individualCommandCallbacks.resize(id + 1);
			_individualCommandCallbacks[id] = std::make_unique<CallbackContainer<T>>(fn);
		}

		template <typename T>
		static void registerSharedCommandCallback(const std::function<void(T &)> &fn)
		{
			auto id = TMQ::Message<T>::getId();
			if (id >= _sharedCommandCallbacks.size())
				_sharedCommandCallbacks.resize(id + 1);
			_sharedCommandCallbacks[id] = std::make_unique<CallbackContainer<T>>(fn);
		}
	protected:
		TMQ::ReleasableQueue _queue;

		AGE::Vector<std::unique_ptr<ICallbackContainer>> _individualCommandCallbacks;
		static AGE::Vector<std::unique_ptr<ICallbackContainer>> _sharedCommandCallbacks;

		friend class CommandQueuePusher;
	};
}