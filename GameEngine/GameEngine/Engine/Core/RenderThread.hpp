#pragma once

#include <Utils/ThreadQueue.hpp>
#include <Context/SdlContext.hh>
#include <Render/RenderManager.hh>
#include <Utils/Dependency.hpp>
#include <Context/IRenderContext.hh>

////
// Default render thread, you can also build yours

namespace AGE
{
	class RenderThread : public ThreadQueue, public Dependency < RenderThread >
	{
		SdlContext *_context;
		gl::RenderManager *_renderManager;


		virtual bool _init();
		virtual bool _initInNewThread();
		virtual bool _release();
		virtual bool _releaseInNewThread();
		virtual bool _update();
	public:
		RenderThread();
		virtual ~RenderThread();

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