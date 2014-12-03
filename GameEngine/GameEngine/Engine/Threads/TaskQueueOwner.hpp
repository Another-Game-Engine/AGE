#pragma once

#include <TMQ/queue.hpp>
#include "CallbackContainer.hpp"
#include <Utils/Containers/Vector.hpp>

namespace AGE
{
	class TaskQueueOwner
	{
	public:
		inline TMQ::ImmediateQueue *getTaskQueue() { return &_queue; }

		template <typename T>
		void registerTaskCallback(const std::function<void(T &)> &fn)
		{
			auto id = TMQ::Message<T>::getId();
			if (id >= _individualCommandCallbacks.size())
				_individualCommandCallbacks.resize(id + 1);
			_individualCommandCallbacks[id] = std::make_unique<CallbackContainer<T>>(fn);
		}

		template <typename T>
		static void registerSharedTaskCallback(const std::function<void(T &)> &fn)
		{
			auto id = TMQ::Message<T>::getId();
			if (id >= _sharedCommandCallbacks.size())
				_sharedCommandCallbacks.resize(id + 1);
			_sharedCommandCallbacks[id] = std::make_unique<CallbackContainer<T>>(fn);
		}

		bool executeTask(TMQ::MessageBase *task);
	private:
		TMQ::ImmediateQueue _queue;

		AGE::Vector<std::unique_ptr<ICallbackContainer>> _individualTaskCallbacks;
		static AGE::Vector<std::unique_ptr<ICallbackContainer>> _sharedTaskCallbacks;
	};
}