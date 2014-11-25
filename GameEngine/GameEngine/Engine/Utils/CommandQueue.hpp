#pragma once

#include <thread>
#include <tmq/templateDispatcher.hpp>
#include <tmq/queue.hpp>
#include <atomic>

namespace AGE
{
	class Engine;
	class CommandQueue
	{
	private:
		TMQ::PtrQueue q;
	protected:
		TMQ::Queue _commandQueue;
		TMQ::Queue *_next;
		CommandQueue *_prev;
		TMQ::Queue _taskQueue;
		Engine *_engine;
		std::atomic_uint64_t _frameNumber;
		std::size_t _threadId;

		struct ICallbackContainer
		{
			virtual void operator()(TMQ::MessageBase *m) = 0;
			virtual ~ICallbackContainer(){}
			ICallbackContainer()
				: _isValid(false)
			{}
			inline bool isValid() { return _isValid; }
		protected:
			bool _isValid;
		};

		template <typename T>
		struct CallbackContainer : public ICallbackContainer
		{
			CallbackContainer()
			{
			}

			CallbackContainer(const std::function<void(T &)> &fn)
				: function(fn)
			{
				_isValid = true;
			}

			virtual void operator()(TMQ::MessageBase *m)
			{
				assert(isValid());
				auto prout = dynamic_cast<TMQ::Message<T>*>(m);
				function(static_cast<TMQ::Message<T>*>(m)->_data);
			}
			virtual ~CallbackContainer(){}

			std::function<void(T &)> function;
		};

		std::vector<std::unique_ptr<ICallbackContainer>> _callbackCollection;

	public:
		CommandQueue(const CommandQueue &) = delete;
		CommandQueue(CommandQueue &&) = delete;
		CommandQueue& operator=(const CommandQueue &) = delete;
		CommandQueue& operator=(CommandQueue &&) = delete;

		CommandQueue()
			: _engine(nullptr)
			, _next(nullptr)
			, _prev(nullptr)
			, _commandQueue(false)
			, _taskQueue(true)
			, _frameNumber(0)
		{
			_taskQueue.setWaitingTime(1);
			_commandQueue.setWaitingTime(1);
		}

		virtual ~CommandQueue()
		{

		}

		// should be done in it's proper thread
		bool init(Engine *engine)
		{
			_engine = engine;
			_threadId = std::this_thread::get_id().hash();
			assert(_engine != nullptr);
			_commandQueue.launch();
			return true;
		}

		// USE ONLY THAT FUNCTION TO GET THE COMMAND QUEUE
		TMQ::Queue *getCommandQueue()
		{
			assert(std::this_thread::get_id().hash() == _threadId);
			return _next;
		}

		TMQ::Queue *getTaskQueue()
		{
			return &_taskQueue;
		}

		void setNextCommandQueue(CommandQueue *next)
		{
			_next = &(next->_commandQueue);
			next->_prev = this;
		}

		// USE THAT FUNCTION ONLY TO PASS THE RESULT to setNextCommandQueue
		// Do NOT use it to pass messages
		TMQ::Queue *getCurrentThreadCommandQueue()
		{
			return &_commandQueue;
		}

		template <typename T>
		void registerMessageCallback(const std::function<void(T &)> &fn)
		{
			auto id = TMQ::Message<T>::getId();
			if (id >= _callbackCollection.size())
				_callbackCollection.resize(id + 1);
			_callbackCollection[id] = std::make_unique<CallbackContainer<T>>(fn);
		}

		bool commandQueueUpdate()
		{
			// We pop task queue
			if (_taskQueue.getReadableQueue(q))
			{
				while (!q.empty())
				{
					auto message = q.front();
					auto id = message->uid;
					if (_callbackCollection.size() <= id || !_callbackCollection[id])
					{
						assert(false);
					}
					(*_callbackCollection[id].get())(message);
					message->_used = true;
					q.pop();
				}
			}
			if (_next->isWritable())
			{
				if (_prev && _prev->_frameNumber != 0)
				{
					if (_commandQueue.getReadableQueue(q))
					{
						while (!q.empty())
						{
							auto message = q.front();
							auto id = message->uid;
							auto tid = message->tid;
							assert(tid != _threadId); // mean that it's this thread who publish the message
							if (_callbackCollection.size() <= id || !_callbackCollection[id])
							{
								if (_next)
								{
									_next->move(message, q.getFrontSize());
									q.pop();
									continue;
								}
								assert(false);
							}
							(*_callbackCollection[id].get())(message);
							message->_used = true;
							q.pop();
						}
						// TODO update
						++_frameNumber;
						if (_frameNumber == 0)
							_frameNumber == 1;
						if (_next)
							_next->releaseReadability();
					}
				}
			}
			else
			{
				// TODO update
				++_frameNumber;
				if (_frameNumber == 0)
					_frameNumber == 1;
			}

			bool isPriorityQueue = this->_commandQueue.getReadableQueue(q);
			if (q.empty())
				return false;
			while (!q.empty())
			{
				auto message = q.front();
				auto id = message->uid;
				auto tid = message->tid;
				assert(tid != _threadId); // mean that it's this thread who publish the message
				if (_callbackCollection.size() <= id || !_callbackCollection[id])
				{
					if (_next)
					{
						if (isPriorityQueue)
							_next->move(message, q.getFrontSize());
						else
							_next->move(message, q.getFrontSize());
						q.pop();
						continue;
					}
					assert(false);
				}
				(*_callbackCollection[id].get())(message);
				message->_used = true;
				q.pop();
			}
			return true;
		}
	};
}