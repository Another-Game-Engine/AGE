#include "RenderThread.hpp"
#include <Core/Engine.hh>
#include <Render/RenderManager.hh>
#include <Context/SdlContext.hh>
#include <Utils/ThreadQueueCommands.hpp>

namespace AGE
{
	RenderThread::RenderThread()
		: Thread(AGE::Thread::ThreadType::Render)
	{}

	RenderThread::~RenderThread()
	{}

	bool RenderThread::init()
	{
		this->registerTaskCallback<MainThreadToRenderThread::CreateRenderContext>([&](MainThreadToRenderThread::CreateRenderContext &msg)
		{
			if (!msg.engine.lock()->setInstance<SdlContext, IRenderContext>()->init(0, 1920, 1040, "~AGE~ V0.00001 Demo"))
			{
				msg.setValue(false);
				return;
			}
			msg.engine.lock()->setInstance<gl::RenderManager>();
			msg.setValue(true);
		});

		//registerTaskCallback<RendCtxCommand::Flush>([&](RendCtxCommand::Flush& msg)
		//{
		//	_context->swapContext();
		//});

		//registerTaskCallback<RendCtxCommand::GetScreenSize>([&](RendCtxCommand::GetScreenSize& msg)
		//{
		//	msg.setValue(_context->getScreenSize());
		//});

		//registerTaskCallback<RendCtxCommand::SetScreenSize>([&](RendCtxCommand::SetScreenSize& msg)
		//{
		//	_context->setScreenSize(msg.screenSize);
		//});

		registerTaskCallback<TQC::BoolFunction>([&](AGE::TQC::BoolFunction& msg)
		{
			msg.setValue(msg.function());
		});

		//registerTaskCallback<RendCtxCommand::RefreshInputs>([&](RendCtxCommand::RefreshInputs& msg)
		//{
		//	_context->refreshInputs();
		//});

		//registerTaskCallback<RendCtxCommand::CopyDrawLists>([&](RendCtxCommand::CopyDrawLists& msg)
		//{
		//	this->_toDrawList = std::move(msg.list);
		//});

		//registerTaskCallback<RendCtxCommand::RenderDrawLists>([&](RendCtxCommand::RenderDrawLists& msg)
		//{
		//	for (auto &e : this->_toDrawList)
		//	{
		//		msg.function(e);
		//	}
		//});

		registerTaskCallback<TQC::VoidFunction>([&](AGE::TQC::VoidFunction& msg)
		{
			if (msg.function)
				msg.function();
		});

#ifdef USE_IMGUI
		registerTaskCallback<AGE::RenderImgui>([&](AGE::RenderImgui &msg)
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

		while (this->_run)
		{
			bool commandsCleared = false;

			if (!getCommandQueue()->getReadableQueue(commands))
			{
				getTaskQueue()->getReadableQueue(tasks);
				while (!tasks.empty())
				{
					//pop all tasks
					auto task = tasks.front();
					assert(executeTask(task)); // we receive a task that we cannot treat
					tasks.pop();
				}
			}
			else
			{
				// pop all commands
				while (!commands.empty())
				{
					auto command = commands.front();
					if (!executeCommand(command))
					{
						/*_activeContext->executeCommand(command);*/ // TO UNCOMMENT
					}
					commands.pop();
				}
			}
		}
		return true;
	}
}