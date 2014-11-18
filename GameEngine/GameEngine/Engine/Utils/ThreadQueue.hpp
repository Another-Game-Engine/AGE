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
		virtual bool _update() = 0;
		virtual bool _init() = 0;
		virtual bool _initInNewThread() = 0;
		virtual bool _release() = 0;
		virtual bool _releaseInNewThread() = 0;
		Engine *_engine;
		std::atomic_bool _run;

		bool update()
		{
			bool run = true;
			run = _initInNewThread();
			auto thisThreadId = std::this_thread::get_id().hash();
			while (_run && run)
			{
				TMQ::PtrQueue q;
				this->_commandQueue.getReadableQueue(q);
				while (!q.empty())
				{
					auto message = q.front();
					auto id = message->uid;
					auto tid = message->tid;
					assert(tid != thisThreadId); // mean that it's this thread who publish the message
					if (_callbackCollection.size() <= id || !_callbackCollection[id])
					{
						// Todo -> push the message for the next thread
						q.pop();
						continue;
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
			_commandQueue.autoPriorityEmplace<TMQ::CloseQueue>();
			_run = false;
			_thread.join();
			_release();
		}

		TMQ::Queue &getCommandQueue()
		{
			return _commandQueue;
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