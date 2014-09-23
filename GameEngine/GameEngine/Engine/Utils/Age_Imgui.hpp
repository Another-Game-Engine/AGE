#pragma once

#include <Configuration.hpp>
#include <Utils/Dependency.hpp>
#include <imgui/imgui.h>
#include <TMQ/queue.hpp>
#include <TMQ/message.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <TMQ/dispatcher.hpp>
#include <TMQ/templateDispatcher.hpp>
#include <map>
#include <memory>

namespace AGE
{
	class Imgui
	{
	private:
		std::map<std::size_t , std::size_t> _threadIds;
		std::map<std::size_t, std::shared_ptr<TMQ::Queue>> _commandQueue;
		std::mutex _mutex;
		static unsigned int _fontTex;
	public:
		Imgui();
		bool init(DependenciesInjector *di);
		void startUpdate();
		void endUpdate();
		void push(const std::function<void()> &fn);
		void push(std::function<void()> &&fn);
		// Call this at the begining of a thread to register it to the Imgui command queue
		void registerThread(std::size_t priority);
		static Imgui* getInstance();
		static void renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count);

	};

#ifdef USE_IMGUI
#define IMGUI_BEGIN ::AGE::Imgui::getInstance()->push([=](){

#define IMGUI_END });
#else
#define IMGUI_BEGIN (void)([=](){

#define IMGUI_END });
#endif

}