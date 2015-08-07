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
			, Render = 1
			, Worker1 = 2
			, Worker2 = 3
			, Worker3 = 4
			, Worker4 = 5
			, Worker5 = 6
			, Worker6 = 7
			, Worker7 = 8
			, Worker8 = 9
			, Worker9 = 10
			, Worker10 = 11
			, Worker11 = 12
			, Worker12 = 13
			, Worker13 = 14
			, Worker14 = 15
			, Worker15 = 16
			, Worker16 = 17
			, END = 18
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
		inline bool isWorker() const { return _worker; }
		void setAsWorker(bool ToF);

	protected:
		// This function will generate the unique id of the thread
		// It have to be called only in the thread context
		std::size_t _registerId();

		const ThreadType _id;
		const std::string _name;
		std::size_t _systemId;
	private:
		std::atomic_bool _worker;
	};
}