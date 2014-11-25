#pragma once
//
//#include <Utils/Dependency.hpp>
//#include <Utils/CommandQueue.hpp>
#include <Core/MainThreadCommand.hpp>
//
//namespace AGE
//{
//	class MainThread : public Dependency < MainThread >, public CommandQueue
//	{
//	public:
//		MainThread()
//			: _next(nullptr)
//			, _thisThreadId(std::this_thread::get_id().hash())
//			, _engine(nullptr)
//		{
//			registerMessageCallback<MTC::FrameTime>([&](MTC::FrameTime& msg)
//			{
//				ImGui::Text((std::string(msg.name) + " " + std::to_string(msg.time)).c_str());
//			});
//
//		}
//
//		bool launch(Engine *engine)
//		{
//			_engine = engine;
//			if (!engine)
//				return false;
//			_commandQueue.launch();
//		}
//
//		// USE ONLY THAT FUNCTION TO GET THE COMMAND QUEUE
//		TMQ::ReleasableQueue *getCommandQueue()
//		{
//			assert(std::this_thread::get_id().hash() == _thisThreadId);
//			return _next;
//		}
//
//		void setNextCommandQueue(TMQ::ReleasableQueue *next)
//		{
//			_next = next;
//		}
//
//		// USE THAT FUNCTION ONLY TO PASS THE RESULT to setNextCommandQueue
//		// Do NOT use it to pass messages
//		TMQ::ReleasableQueue *getCurrentThreadCommandQueue()
//		{
//			return &_commandQueue;
//		}
//
//	private:
//		TMQ::ReleasableQueue _commandQueue;
//		TMQ::ReleasableQueue *_next;
//		std::size_t _thisThreadId;
//		Engine *_engine;
//	};
//}