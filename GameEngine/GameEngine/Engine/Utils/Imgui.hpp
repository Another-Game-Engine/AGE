#pragma once

#include <Configuration.hpp>
#include <Utils/Dependency.hpp>
#include <imgui/imgui.h>
#include <TMQ/queue.hpp>
#include <TMQ/message.hpp>
#include <Utils/ThreadQueueCommands.hpp>

#ifdef USE_IMGUI

namespace AGE
{
	class Imgui : public Dependency < Imgui >
	{
	private:
		TMQ::Queue _commandQueue;
	public:
		void update()
		{
			_commandQueue.releaseReadability();
			_commandQueue.getDispatcher()
				.handle<TQC::VoidFunction>([&](const TQC::VoidFunction& msg)
			{
				msg.function();
			});
		}

		void push(const std::function<void()> &fn)
		{
			_commandQueue.autoEmplace<TQC::VoidFunction>(fn);
		}

		void push(std::function<void()> &&fn)
		{
			_commandQueue.autoEmplace<TQC::VoidFunction>(fn);
		}
	};
}

#endif