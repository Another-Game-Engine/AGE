#pragma once

#include <atomic>

namespace AGE
{
	class Thread
	{
	public:
		enum ThreadType : std::size_t
		{
			Main = 0
			, PrepareRender
			, Render
			, Worker1
			, Worker2
			, Worker3
			, Worker4
			, Worker5
			, END
		};
		virtual bool init() = 0;
		virtual bool release() = 0;
		Thread(ThreadType type);
		virtual ~Thread();
		Thread() = delete;
		Thread(Thread &) = delete;
		Thread(Thread &&) = delete;
		Thread &operator=(Thread &) = delete;
		Thread &operator=(Thread &&) = delete;
		inline std::size_t getId() const { return _id; }
	protected:
		// This function will generate the unique id of the thread
		// It have to be called only in the thread context
		std::size_t _registerId();

		const ThreadType _id;
	};
}