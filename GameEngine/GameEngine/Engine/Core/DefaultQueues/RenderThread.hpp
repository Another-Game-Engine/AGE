#pragma once

#include <Utils/ThreadQueue.hpp>
#include <Context/SdlContext.hh>
#include <Render/RenderManager.hh>
#include <Utils/Dependency.hpp>
#include <Context/IRenderContext.hh>

namespace AGE
{
	namespace DefaultQueue
	{
		class RenderThread : public ThreadQueue, public Dependency < RenderThread >
		{
			SdlContext *_context;
			gl::RenderManager *_renderManager;


			virtual bool _init()
			{
				_context = _engine->setInstance<SdlContext, IRenderContext>();
				_renderManager = _engine->setInstance<gl::RenderManager>();
				return true;
			}

			virtual bool _initInNewThread()
			{
				return true;
			}

			virtual bool _release()
			{
				return true;
			}

			virtual bool _releaseInNewThread()
			{
				_engine->deleteInstance<IRenderContext>();
				_engine->deleteInstance<gl::RenderManager>();
				return true;
			}

			virtual bool _update()
			{
				bool returnValue = true;

				_commandQueue.getDispatcher()
					.handle<RendCtxCommand::Flush>([&](const RendCtxCommand::Flush& msg)
				{
					_context->swapContext();
				})
					.handle<RendCtxCommand::GetScreenSize>([&](RendCtxCommand::GetScreenSize& msg)
				{
					msg.result.set_value(_context->getScreenSize());
				})
					.handle<RendCtxCommand::SetScreenSize>([&](const RendCtxCommand::SetScreenSize& msg)
				{
					_context->setScreenSize(msg.screenSize);
				})
					.handle<BoolFunction>([&](BoolFunction& msg)
				{
					msg.result.set_value(msg.function());
				})
					.handle<RendCtxCommand::RefreshInputs>([&](const RendCtxCommand::RefreshInputs& msg)
				{
					_context->refreshInputs();
				})
				.handle<VoidFunction>([&](const VoidFunction& msg)
				{
					if (msg.function)
						msg.function();
					else
						std::cout << "Prout" << std::endl;
				})
				.handle<Stop>([&](Stop& msg)
				{
					returnValue = false;
				});

				return returnValue;
			}

		public:
			RenderThread()
			{}

			virtual ~RenderThread()
			{}


			/////////////////////
			/// COMMANDS
			struct BoolFunction : public TMQ::FutureData < bool >
			{
				std::function<bool()> function;
				BoolFunction(const std::function<bool()> &_function)
					: function(_function)
				{
				}
				virtual ~BoolFunction()
				{}
			};

			struct VoidFunction
			{
				std::function<void(void)> function;
				VoidFunction(const std::function<void(void)> &_function)
					: function(_function)
				{
				}
				VoidFunction(const VoidFunction &) = delete;
				VoidFunction &operator=(const VoidFunction&) = delete;
				~VoidFunction()
				{
				}
			};

			struct Stop{};

		};
	}
}