#include "RenderThread.hpp"
#include <Core/Engine.hh>
#include <Render/RenderManager.hh>
#include <Context/SdlContext.hh>
#include <Utils/ThreadQueueCommands.hpp>
#include <Utils/ThreadName.hpp>
#include <Core/Tasks/Render.hpp>
#include <Core/Commands/Render.hpp>
#include <Context/SdlContext.hh>
#include <Utils/Containers/Vector.hpp>
#include <Core/CullableObjects.hh>
#include <Core/Tasks/Basics.hpp>

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
		, _context(nullptr)
		, _render(nullptr)
	{}

	RenderThread::~RenderThread()
	{}

	bool RenderThread::init()
	{
		this->registerCallback<Tasks::Render::CreateRenderContext>([this](Tasks::Render::CreateRenderContext &msg)
		{
			_context = msg.engine.lock()->setInstance<SdlContext, IRenderContext>();
			if (!_context->init(0, 1920, 1040, "~AGE~ V0.00001 Demo"))
			{
				msg.setValue(false);
				return;
			}
			_render = msg.engine.lock()->setInstance<gl::RenderManager>();
			msg.setValue(true);
		});

 		registerCallback<Commands::Render::Flush>([&](Commands::Render::Flush& msg)
		{
			_context->swapContext();

		});

		registerCallback<Tasks::Render::GetWindowSize>([&](Tasks::Render::GetWindowSize &msg)
		{
			msg.setValue(_context->getScreenSize());
		});

		registerCallback<Tasks::Render::SetWindowSize>([&](Tasks::Render::SetWindowSize& msg)
		{
			_context->setScreenSize(msg.size);
		});

		registerCallback<Commands::Render::CopyDrawLists>([&](Commands::Render::CopyDrawLists& msg)
		{
			//this->_drawlist.insert(this->_drawlist.end(), msg.list.begin(), msg.list.end());
			this->_drawlist = msg.list;
		});

		registerCallback<Commands::Render::RenderDrawLists>([&](Commands::Render::RenderDrawLists& msg)
		{
			for (auto &e : this->_drawlist)
			{
				msg.function(e);
			}
		});

		registerSharedCallback<AGE::Tasks::Basic::BoolFunction>([&](AGE::Tasks::Basic::BoolFunction& msg)
		{
			msg.setValue(msg.function());
		});

		registerCallback<AGE::Tasks::Basic::VoidFunction>([&](AGE::Tasks::Basic::VoidFunction& msg)
		{
			if (msg.function)
				msg.function();
		});

#ifdef USE_IMGUI
		registerCallback<AGE::RenderImgui>([&](AGE::RenderImgui &msg)
		{
			AGE::Imgui::getInstance()->renderThreadRenderFn(msg.cmd_lists);
		});
#endif

		return true;
	}

	bool RenderThread::release()
	{
		return true;
	}

	bool RenderThread::launch()
	{
		if (!init())
			return false;
		_threadHandle = std::thread(&RenderThread::update, std::ref(*this));
		return true;
	}

	bool RenderThread::stop()
	{
		_run = false;
		_threadHandle.join();
		return true;
	}

	bool RenderThread::update()
	{
		/*
		- Tant qu'il n'y a pas de command
		-> je pop des task
		- Une fois que j'ai mes commandes
		-> pour chacunes d'elles
		-> je regarde si c'est a moi de les traiter (ex : changement de scene)
		-> si ca n'est pas a moi de les traiter
		-> je les passe au render context actif
		-> si il n'y a pas de render context actif, c'est que j'ai fait une erreur, et j'assert dans ta face :D
		*/

		_registerId();

		this->_run = true;
		DWORD threadId = GetThreadId(static_cast<HANDLE>(_threadHandle.native_handle()));
		SetThreadName(threadId, this->_name.c_str());

		TMQ::PtrQueue commands;
		TMQ::PtrQueue tasks;
		bool commandSuccess;
		bool taskSuccess;

		while (this->_run)
		{
			if (_context)
				_context->refreshInputs();
			getQueue()->getTaskAndCommandQueue(tasks, taskSuccess, commands, commandSuccess, TMQ::HybridQueue::Block);
			if (taskSuccess)
			{
				while (!tasks.empty())
				{
					//pop all tasks
					auto task = tasks.front();
					assert(execute(task)); // we receive a task that we cannot treat
					tasks.pop();
				}
			}
			if (commandSuccess)
			{
				// pop all commands
				while (!commands.empty())
				{
					auto command = commands.front();
					assert(execute(command));
					commands.pop();
				}
				_drawlist.clear();
			}
		}
		return true;
	}
}

//bool OldRenderThread::_updateBegin()
//{
//	bool returnValue = true;
//	return returnValue;
//}
//
//bool OldRenderThread::_updateEnd()
//{
//	bool returnValue = true;
//	_next->getTaskQueue()->emplace<AGE::MTC::FrameTime>(this->_threadId, std::chrono::duration_cast<std::chrono::milliseconds>(_elapsed).count());
//	return returnValue;
//}