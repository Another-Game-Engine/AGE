#pragma once

#include <Configuration.hpp>
#include <Utils/Dependency.hpp>
#include <imgui/imgui.h>
#include <TMQ/queue.hpp>
#include <TMQ/message.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <TMQ/dispatcher.hpp>
#include <TMQ/templateDispatcher.hpp>

namespace AGE
{
	class Imgui
	{
	private:
		TMQ::Queue _commandQueue;
	public:
		Imgui();

		bool init(DependenciesInjector *di);

		void update()
		{
#ifdef USE_IMGUI
			_commandQueue.releaseReadability();
			_commandQueue.getDispatcher()
				.handle<TQC::VoidFunction>([&](const TQC::VoidFunction& msg)
			{
				msg.function();
			});
#endif
		}

		void push(const std::function<void()> &fn)
		{
#ifdef USE_IMGUI
			_commandQueue.autoEmplace<TQC::VoidFunction>(fn);
#endif
		}

//		void push(std::function<void()> &&fn)
//		{
//#ifdef USE_IMGUI
//			_commandQueue.autoEmplace<TQC::VoidFunction>(fn);
//#endif
//		}

		static Imgui* getInstance()
		{
			static Imgui* ImguiInstance = new Imgui();
			return ImguiInstance;
		}

	};

#define IMGUI_BEGIN AGE::Imgui::getInstance()->push([=](){

#define IMGUI_END });

}