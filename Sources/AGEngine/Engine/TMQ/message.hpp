#pragma once

#include <future>

namespace TMQ
{
	struct MessageBase
	{
		virtual ~MessageBase();
		MessageBase(std::size_t _uid);
		const std::size_t uid;
		MessageBase(const MessageBase&o) = delete;
		MessageBase &operator=(const MessageBase&o) = delete;
		MessageBase(MessageBase&&o) = delete;
		MessageBase &operator=(MessageBase&&o) = delete;
	protected:
		static std::size_t __sharedIdCounter;
	};

	template <typename T>
	struct Message : public MessageBase
	{
		T _data;

		virtual ~Message()
		{
		}

		static std::size_t getId()
		{
			static std::size_t id = MessageBase::__sharedIdCounter++;
			return id;
		}

		explicit Message(const T &data)
			: MessageBase(getId()), _data(data)
		{}

		explicit Message(T &&data)
			: MessageBase(getId()), _data(std::move(data))
		{}

		template <typename ...Args>
		explicit Message(Args ...args)
			: MessageBase(getId()), _data(args...)
		{
		}

		Message &operator=(const Message&o) = delete;
		Message(const Message&o) = delete;
		inline T &getData() { return _data; }
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