#include "Thread.hpp"
#include "ThreadManager.hpp"

#include <thread>

namespace AGE
{
	Thread::Thread(Thread::ThreadType type)
		: _id(type)
		, _name(threadTypeToString(type))
		, _systemId(0)
	{
		taskCounter = 0;
	}

	Thread::~Thread()
	{

	}

	std::size_t Thread::_registerId()
	{
		_systemId = std::this_thread::get_id().hash();
		Singleton<ThreadManager>::getInstance()->registerThreadId(_systemId, _id);
		SetCurrentThread(this);
		return _id;
	}

	std::string Thread::threadTypeToString(ThreadType t)
	{
		std::string res;
		switch (t)
		{
		case AGE::Thread::Main:
			res = "MainThread";
			break;
		case AGE::Thread::Render:
			res = "AGE_RenderThread";
			break;
		case AGE::Thread::Worker1:
			res = "AGE_WorkerThread1";
			break;
		case AGE::Thread::Worker2:
			res = "AGE_WorkerThread2";
			break;
		case AGE::Thread::Worker3:
			res = "AGE_WorkerThread3";
			break;
		case AGE::Thread::Worker4:
			res = "AGE_WorkerThread4";
			break;
		case AGE::Thread::Worker5:
			res = "AGE_WorkerThread5";
			break;
		case AGE::Thread::Worker6:
			res = "AGE_WorkerThread6";
			break;
		case AGE::Thread::Worker7:
			res = "AGE_WorkerThread7";
			break;
		case AGE::Thread::Worker8:
			res = "AGE_WorkerThread8";
			break;
		case AGE::Thread::Worker9:
			res = "AGE_WorkerThread9";
			break;
		case AGE::Thread::Worker10:
			res = "AGE_WorkerThread10";
			break;
		case AGE::Thread::Worker11:
			res = "AGE_WorkerThread11";
			break;
		case AGE::Thread::Worker12:
			res = "AGE_WorkerThread12";
			break;
		case AGE::Thread::Worker13:
			res = "AGE_WorkerThread13";
			break;
		case AGE::Thread::Worker14:
			res = "AGE_WorkerThread14";
			break;
		case AGE::Thread::Worker15:
			res = "AGE_WorkerThread15";
			break;
		case AGE::Thread::Worker16:
			res = "AGE_WorkerThread16";
			break;
		case AGE::Thread::END:
			assert(false); // Illegal thread type
			break;
		default:
			assert(false); // Illegal thread type
			break;
		}
		return res;
	}

	std::size_t Thread::hardwareConcurency()
	{
		static std::size_t res = std::thread::hardware_concurrency();
		if (res <= 3)
		{
			printf("Warning : your computer can only support %i threads ! AGE is designed to use at least 4 threads.", res);
			res = 4;
		}
		return res;
	}

}
