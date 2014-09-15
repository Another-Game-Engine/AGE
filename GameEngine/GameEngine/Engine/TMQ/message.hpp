#pragma once

#include <future>

namespace TMQ
{
	struct MessageBase
	{
		virtual ~MessageBase();
		MessageBase(std::size_t _uid);
		std::size_t uid;
		MessageBase(const MessageBase&) = delete;
		MessageBase &operator=(const MessageBase&) = delete;
	};

	template <typename T>
	struct Message : public MessageBase
	{
		T _data;

		virtual ~Message()
		{}

		static std::size_t getId()
		{
			static std::size_t id = typeid(T).hash_code();
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

		Message &operator=(const Message&) = delete;
		Message(const Message&) = delete;
	};

	// Used for messages which return value
	// ex : struct MyReturnValueMsg : public FutureData<int>
	template <typename T>
	struct FutureData
	{
		virtual ~FutureData()
		{}

		std::promise<T> result;
		std::future<T> getFuture()
		{
			return result.get_future();
		}
		FutureData& operator=(const FutureData&) = delete;
		explicit FutureData(const FutureData&) = delete;
		explicit FutureData() = default;
		FutureData& operator=(FutureData&& o)
		{
			result = std::move(o.result);
			return *this;
		}
		explicit FutureData(FutureData&& o)
		{
			result = std::move(o.result);
		}
	};

	//TODO REMOVE
	class CloseQueue
	{
	};
}