#pragma once

#include <thread>
#include <tmq/templateDispatcher.hpp>
#include <tmq/queue.hpp>
#include <Core/Engine.hh>
namespace AGE
{
	class ThreadQueue
	{
	protected:
		std::thread _thread;
		TMQ::Queue _commandQueue;
		TMQ::Queue *_next;
		virtual bool _update() = 0;
		virtual bool _init() = 0;
		virtual bool _initInNewThread() = 0;
		virtual bool _release() = 0;
		virtual bool _releaseInNewThread() = 0;
		Engine *_engine;
		std::atomic_bool _run;
		std::size_t _thisThreadId;

		bool update()
		{
			bool run = true;
			run = _initInNewThread();
			_thisThreadId = std::this_thread::get_id().hash();
			while (_run && run)
			{
				TMQ::PtrQueue q;
				this->_commandQueue.getReadableQueue(q);
				while (!q.empty())
				{
					auto message = q.front();
					auto id = message->uid;
					auto tid = message->tid;
					assert(tid != _thisThreadId); // mean that it's this thread who publish the message
					if (_callbackCollection.size() <= id || !_callbackCollection[id])
					{
						// Todo -> push the message for the next thread
						if (_next)
						{
							_next->autoMove(message, q.getFrontSize());
							q.pop();
							continue;							
						}
						assert(false);
					}
					(*_callbackCollection[id].get())(message);
					q.pop();
				}
				run = _update();
			}
			return _releaseInNewThread();
		}

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

	public:
		ThreadQueue(const ThreadQueue &) = delete;
		ThreadQueue(ThreadQueue &&) = delete;
		ThreadQueue& operator=(const ThreadQueue &) = delete;
		ThreadQueue& operator=(ThreadQueue &&) = delete;

		ThreadQueue()
			: _engine(nullptr)
			, _next(nullptr)
		{}

		virtual ~ThreadQueue()
		{

		}

		bool launch(Engine *engine)
		{
			_run = true;
			_engine = engine;
			assert(_engine != nullptr);
			auto res = _init();
			_commandQueue.launch();
			_thread = std::thread(&ThreadQueue::update, std::ref(*this));
			return res;
		}

		void quit()
		{
			if (!_thread.joinable())
				return;
			_run = false;
			_thread.join();
			_release();
		}

		// USE ONLY THAT FUNCTION TO GET THE COMMAND QUEUE
		TMQ::Queue *getCommandQueue()
		{
			assert(std::this_thread::get_id().hash() == _thisThreadId);
			return _next;
		}

		void setNextCommandQueue(TMQ::Queue *next)
		{
			_next = next;
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
	};
}