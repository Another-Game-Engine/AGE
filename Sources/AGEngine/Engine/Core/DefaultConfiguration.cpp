#include "DefaultConfiguration.hpp"
#include <Configuration.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Core/Engine.hh>
#include <Core/Age_Imgui.hpp>
#include <Core/Timer.hh>

namespace AGE
{
	std::function<bool()> DefaultEngineConfiguration(Engine *engine, std::function<bool()> &userConfigs)
	{
		return std::function<bool()>([&](){
			#ifdef AGE_ENABLE_IMGUI
			TMQ::TaskManager::emplaceRenderFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=](){
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