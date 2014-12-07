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
			if (id >= _individualTaskCallbacks.size())
				_individualTaskCallbacks.resize(id + 1);
			_individualTaskCallbacks[id] = std::make_unique<CallbackContainer<T>>(fn);
		}

		template <typename T>
		static void registerSharedTaskCallback(const std::function<void(T &)> &fn)
		{
			auto id = TMQ::Message<T>::getId();
			if (id >= _sharedTaskCallbacks.size())
				_sharedTaskCallbacks.resize(id + 1);
			_sharedTaskCallbacks[id] = std::make_unique<CallbackContainer<T>>(fn);
		}

		bool executeTask(TMQ::MessageBase *task);
	private:
		TMQ::HybridQueue _queue;

		AGE::Vector<std::unique_ptr<ICallbackContainer>> _individualTaskCallbacks;
		static AGE::Vector<std::unique_ptr<ICallbackContainer>> _sharedTaskCallbacks;
	};
}