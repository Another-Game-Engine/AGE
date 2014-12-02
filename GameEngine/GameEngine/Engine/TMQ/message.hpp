#pragma once

#include <future>

namespace TMQ
{
	struct MessageBase
	{
		virtual ~MessageBase();
		MessageBase(std::size_t _uid, std::size_t _tid);
		std::size_t uid;
		std::size_t tid; // thread id
		MessageBase(const MessageBase&o)
		{
			uid = o.uid;
			tid = o.tid;
			_used = o._used;
		}
		MessageBase &operator=(const MessageBase&o)
		{
			uid = o.uid;
			tid = o.tid;
			_used = o._used;
			return *this;
		}
		MessageBase(MessageBase&&o)
		{
			uid = o.uid;
			tid = o.tid;
			_used = o._used;
		}
		MessageBase &operator=(MessageBase&&o)
		{
			uid = o.uid;
			tid = o.tid;
			_used = o._used;
			return *this;
		}
		bool _used = false;
		MessageBase *clone(char* dest) { return _clone(dest); }
	protected:
		static std::size_t __sharedIdCounter;
		virtual MessageBase *_clone(char *dest) = 0;
	};

	template <typename T>
	struct Message : public MessageBase
	{
		T _data;

		virtual ~Message()
		{}

		static std::size_t getId()
		{
			static std::size_t id = MessageBase::__sharedIdCounter++;
			return id;
		}

		explicit Message(const T &data)
			: MessageBase(getId(), std::this_thread::get_id().hash()), _data(data)
		{}

		explicit Message(T &&data)
			: MessageBase(getId(), std::this_thread::get_id().hash()), _data(std::move(data))
		{}

		template <typename ...Args>
		explicit Message(Args &...args)
			: MessageBase(getId(), std::this_thread::get_id().hash()), _data(args...)
		{
		}

		Message &operator=(const Message&o)
		{
			if (&o != this)
			{
				_data = o._data;
			}
			return *this;
		}
		Message(const Message&o)
		{
			if (&o != this)
			{
				_data = o._data;
			}
		}

		virtual MessageBase *_clone(char *dest)
		{
			return new(dest)Message<T>(_data);
		}
	};

	// Used for messages which return value
	// ex : struct MyReturnValueMsg : public FutureData<int>
	template <typename T>
	struct FutureData
	{
	private:
		std::promise<T> *result;
		bool _hasBeenUsed;
	public:
		virtual ~FutureData<T>()
		{
			if (_hasBeenUsed)
				delete result;
		}

		std::future<T> getFuture()
		{
			return result->get_future();
		}
		explicit FutureData()
			: result(nullptr)
			, _hasBeenUsed(false)
		{
			result = new std::promise<T>();
		}
		FutureData& operator=(FutureData&& o)
		{
			result = std::move(o.result);
			_hasBeenUsed = o._hasBeenUsed;
			return *this;
		}
		explicit FutureData(FutureData&& o)
		{
			result = std::move(o.result);
			_hasBeenUsed = o._hasBeenUsed;
		}
		FutureData& operator=(const FutureData&o)
		{
			result = o.result;
			_hasBeenUsed = o._hasBeenUsed;
			return *this;
		}
		explicit FutureData(const FutureData&o)
		{
			result = o.result;
			_hasBeenUsed = o._hasBeenUsed;
		}

		void setValue(const T &v)
		{
			_hasBeenUsed = true;
			result->set_value(v);
		}
	};
}