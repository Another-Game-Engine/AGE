#include "DefaultConfiguration.hpp"
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Core/Timer.hh>
#include <Threads/Tasks/BasicTasks.hpp>

namespace AGE
{
	std::function<bool()> DefaultEngineConfiguration(Engine *engine, std::function<bool()> &userConfigs)
	{
		return std::function<bool()>([&](){
			#ifdef AGE_ENABLE_IMGUI
			GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=](){
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