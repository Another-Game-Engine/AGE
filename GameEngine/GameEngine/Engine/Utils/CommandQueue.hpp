#pragma once

#include <thread>
#include <tmq/templateDispatcher.hpp>
#include <tmq/queue.hpp>
#include <atomic>
#include <Core/MainThreadCommand.hpp>
#include <iostream>

namespace AGE
{
	class Engine;
	class CommandQueue
	{
	private:
		TMQ::PtrQueue q;
	protected:
		TMQ::ReleasableQueue _commandQueue;
		TMQ::ImmediateQueue _taskQueue;
		CommandQueue *_next;
		CommandQueue *_prev;
		Engine *_engine;
		std::size_t _threadId;
		std::string _name;
		std::chrono::system_clock::time_point _time;
		std::chrono::system_clock::duration _elapsed;

		virtual bool _init() = 0;
		virtual bool _initInNewThread() = 0;
		virtual bool _release() = 0;
		virtual bool _releaseInNewThread() = 0;
		virtual bool _updateBegin() = 0;
		virtual bool _updateEnd() = 0;

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
				function(static_cast<TMQ::Message<T>*>(m)->_data);
			}
			virtual ~CallbackContainer(){}

			std::function<void(T &)> function;
		};

		std::vector<std::unique_ptr<ICallbackContainer>> _callbackCollection;

		bool updateBegin()
		{
			//_time = std::chrono::system_clock::now();
			return _updateBegin();
		}

		bool updateEnd()
		{
			bool res =_updateEnd();
			if (!res)
				return res;
			//_elapsed = std::chrono::system_clock::now() - _time;
			return res;
		}

	public:
		bool _hasFrameBefore;
		CommandQueue(const CommandQueue &) = delete;
		CommandQueue(CommandQueue &&) = delete;
		CommandQueue& operator=(const CommandQueue &) = delete;
		CommandQueue& operator=(CommandQueue &&) = delete;

		CommandQueue()
			: _engine(nullptr)
			, _next(nullptr)
			, _prev(nullptr)
		{
			_commandQueue.setWaitingTime(1);
			_hasFrameBefore = false;
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
			return _init();
		}

		// USE ONLY THAT FUNCTION TO GET THE COMMAND QUEUE
		TMQ::ReleasableQueue *getCommandQueue()
		{
			assert(std::this_thread::get_id().hash() == _threadId);
			return _next->getCurrentThreadCommandQueue();
		}

		TMQ::ImmediateQueue *getTaskQueue()
		{
			return &_taskQueue;
		}

		void setNextCommandQueue(CommandQueue *next)
		{
			_next = next;
			next->_prev = this;
		}

		// USE THAT FUNCTION ONLY TO PASS THE RESULT to setNextCommandQueue
		// Do NOT use it to pass messages
		TMQ::ReleasableQueue *getCurrentThreadCommandQueue()
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
						assert(false); // || return false
					}
					(*_callbackCollection[id].get())(message);
					message->_used = true;
					q.pop();
				}
			}


			// If commands are released
			// 1 : This is the first frame of the main thread
			// 2 : There were a frame before who release it
			if (!_hasFrameBefore)
				return true;
			updateBegin();
			if (_commandQueue.getReadableQueue(q))
			{
				while (!q.empty())
				{
					auto message = q.front();
					auto id = message->uid;
					auto tid = message->tid;
					assert(tid != _threadId);
					if (_callbackCollection.size() <= id || !_callbackCollection[id])
					{
						_next->getCurrentThreadCommandQueue()->move(message, q.getFrontSize());
						q.pop();
						continue;
						assert(false);
					}
					(*_callbackCollection[id].get())(message);
					message->_used = true;
					q.pop();
				}
			}
			updateEnd();
			_next->_hasFrameBefore = true;
//			auto t = std::chrono::system_clock::now();
			while (_next->getCurrentThreadCommandQueue()->releaseReadability() == false)
				;
//			auto tt = std::chrono::system_clock::now() - t;
//			std::cout << _name + " : " + std::to_string(tt.count()) << std::endl;
			return true;
		}
	};
}