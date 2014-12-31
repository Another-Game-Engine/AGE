#pragma once

#include <atomic>
#include <string>

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

		static std::string threadTypeToString(ThreadType t);
		static std::size_t hardwareConcurency();

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
		inline std::size_t getSystemId() const { return _systemId; }
		virtual bool launch() = 0;
		virtual bool stop() = 0;
		std::atomic_size_t taskCounter;
	protected:
		// This function will generate the unique id of the thread
		// It have to be called only in the thread context
		std::size_t _registerId();

		const ThreadType _id;
		const std::string _name;
		std::size_t _systemId;
	};
}