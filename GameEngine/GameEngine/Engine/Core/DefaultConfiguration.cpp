#include "DefaultConfiguration.hpp"
#include <Core/Engine.hh>
#include <Render/RenderManager.hh>
#include <Configuration.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Core/Timer.hh>

namespace AGE
{
	std::function<bool()> DefaultEngineConfiguration(Engine *engine, std::function<bool()> &userConfigs)
	{
		return std::function<bool()>([&](){
			#ifdef USE_IMGUI
				GetRenderThread()->getTaskQueue()->emplaceFuture<AGE::TQC::BoolFunction, bool>([=](){
					AGE::Imgui::getInstance()->init(engine);
					return true;
				}).get();
			#endif
				engine->setInstance<Timer>();
				if (userConfigs)
					userConfigs();
				return true;
			// launch engine
			//e->getCurrentThreadCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait);
			//e->_hasFrameBefore = true;
			//e->getCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait);
			//e->first = true;
			//e->getRenderThread()->last = true;			
		});
	}
}