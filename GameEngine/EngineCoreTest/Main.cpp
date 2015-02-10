#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>
#include <stdint.h>

#include <stdlib.h>
#include <Core/Engine.hh>

// SCENES
#include <Scenes/BenchmarkScene.hpp>

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Timer.hh>
#include <Utils/PerformanceDebugger.hh>
#include <AssetManagement/AssetManager.hh>

#include <Utils/Age_Imgui.hpp>
#include <Skinning/AnimationManager.hpp>
//CONFIGS
#include <CONFIGS.hh>

////////////////////////////////////////
/////// NEW IMPLEMENTATION
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Core/DefaultConfiguration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
////////////////////////////////////////

#include <chrono>

using namespace AGE;

int			main(int ac, char **av)
{
	///////////////////////////////////////////////////////////////////////////////////
	/////////// NEW IMPLEMENTATION
	///////////
	AGE::InitAGE();
	auto engine = AGE::CreateEngine();

	engine.lock()->launch(std::function<bool()>([&]()
	{
		AGE::GetThreadManager()->setAsWorker(true, true, false);
		engine.lock().get()->setInstance<Timer>();
		engine.lock().get()->setInstance<AGE::AssetsManager>();

#ifdef USE_IMGUI
		AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=](){
			AGE::Imgui::getInstance()->init(engine.lock().get());
			return true;
		}).get();
#endif
		// add main scene
		engine.lock()->addScene(std::make_shared<BenchmarkScene>(engine), "BenchmarkScene");
//		engine.lock()->addScene(std::make_shared<BenchmarkScene>(engine), "BenchmarkScene2");
		// bind scene
		if (!engine.lock()->initScene("BenchmarkScene"))
			return false;
		//if (!engine.lock()->initScene("BenchmarkScene2"))
		//	return false;
		engine.lock()->enableScene("BenchmarkScene", 100);
//		engine.lock()->enableScene("BenchmarkScene2", 101);
		return true;
	}));
	return (EXIT_SUCCESS);
}
