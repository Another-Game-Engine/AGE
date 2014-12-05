#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>
#include <stdint.h>

#include <stdlib.h>
#include <Core/Engine.hh>

#include <Render/RenderManager.hh>
// SCENES
#include <Scenes/BenchmarkScene.hpp>

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Timer.hh>
#include <Utils/PerformanceDebugger.hh>
#include <Core/AssetsManager.hpp>
#include <Core/MainThread.hpp>
#include <Core/RenderThread.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <Skinning/AnimationManager.hpp>
//CONFIGS
#include <CONFIGS.hh>

////////////////////////////////////////
/////// NEW IMPLEMENTATION
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Core/DefaultConfiguration.hpp>
#include <Threads/RenderThread.hpp>
////////////////////////////////////////

using namespace AGE;

bool loadAssets(AGE::Engine *e)
{
	return AGE::GetRenderThread()->getTaskQueue()->emplaceFuture<AGE::TQC::BoolFunction, bool>([=](){

		e->getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/AGE-Assets-For-Test/Serialized/");
#ifdef RENDERING_ACTIVATED
		e->getInstance<AGE::AssetsManager>()->loadMesh(File("cube/cube.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents });
		e->getInstance<AGE::AssetsManager>()->loadMaterial(File("cube/cube.mage"));
		e->getInstance<AGE::AssetsManager>()->loadMesh(File("ball/ball.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents });
		e->getInstance<AGE::AssetsManager>()->loadMaterial(File("ball/ball.mage"));
		e->getInstance<AGE::AssetsManager>()->loadMesh(File("catwoman/catwoman.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents });
		e->getInstance<AGE::AssetsManager>()->loadMaterial(File("catwoman/catwoman.mage"));
		e->getInstance<AGE::AssetsManager>()->loadSkeleton(File("catwoman/catwoman.skage"));
		e->getInstance<AGE::AssetsManager>()->loadAnimation(File("catwoman/catwoman-roulade.aage"));

		e->getInstance<AGE::AssetsManager>()->loadMesh(File("sponza/sponza.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents });

		e->getInstance<AGE::AssetsManager>()->loadMaterial(File("sponza/sponza.mage"));
		return true;
#endif
	}).get();
}

int			main(int ac, char **av)
{
	///////////////////////////////////////////////////////////////////////////////////
	/////////// NEW IMPLEMENTATION
	///////////
	AGE::InitAGE();
	auto engine = AGE::CreateEngine();

	engine.lock()->launch(std::function<bool()>([&]()
	{
		engine.lock().get()->setInstance<Timer>();
		engine.lock().get()->setInstance<AGE::AssetsManager>();


#ifdef USE_IMGUI
		AGE::GetRenderThread()->getTaskQueue()->emplaceFuture<AGE::TQC::BoolFunction, bool>([=](){
			AGE::Imgui::getInstance()->init(engine.lock().get());
			return true;
		}).get();
#endif
#ifdef RENDERING_ACTIVATED
		if (!loadAssets(engine.lock().get()))
			return false;
#endif
		// add main scene
		engine.lock()->addScene(std::make_shared<BenchmarkScene>(engine), "BenchmarkScene");

		// bind scene
		if (!engine.lock()->initScene("BenchmarkScene"))
			return false;

		engine.lock()->enableScene("BenchmarkScene", 100);
		return true;
	}));


	//return std::function<bool()>([&](){
	//		if (userConfigs)
	//			userConfigs();
	//		return true;

	//	engine.lock()->launch(AGE::DefaultEngineConfiguration(engine.lock().get(), std::function<bool()>([&]()
	//	{
	//
	//#ifdef RENDERING_ACTIVATED
	//		if (!loadAssets(engine.lock().get()))
	//			return false;
	//#endif
	//		// add main scene
	//		engine.lock()->addScene(std::make_shared<BenchmarkScene>(engine), "BenchmarkScene");
	//		
	//		// bind scene
	//		if (!engine.lock()->initScene("BenchmarkScene"))
	//			return false;
	//		
	//		engine.lock()->enableScene("BenchmarkScene", 100);
	//		return true;
	//	})));



	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	//	std::shared_ptr<AGE::Engine>	e = std::make_shared<AGE::Engine>();
	//	e->init(e.get());
	//
	//	// Set Configurations
	//	auto config = e->setInstance<ConfigurationManager>(File("NewMyConfigurationFile.conf"));
	//
	//	auto context = e->getInstance<IRenderContext>();
	//	auto renderManager = e->getInstance<gl::RenderManager>();
	//
	//	auto contextInit = e->getRenderThread()->getTaskQueue()->emplaceFuture<AGE::TQC::BoolFunction, bool>(
	//		std::function<bool()>([&]()
	//	{
	//		if (!context->init(0, 1920, 1040, "~AGE~ V0.0 Demo"))
	//			return false;
	//#ifdef RENDERING_ACTIVATED
	//		if (!loadAssets(e.get()))
	//			return false;
	//#endif
	//		return true;
	//	}));
	//
	//	e->setInstance<PerformanceDebugger>("Developer Name");
	//
	//#ifdef PHYSIC_SIMULATION
	//	e->setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
	//#endif
	//
	//	// Set default window size
	//	// If config file has different value, it'll be changed automatically
	//	config->setConfiguration<glm::uvec2>("windowSize", glm::uvec2(1920, 1040), [&](const glm::uvec2 &v)
	//	{
	//		e->getMainThread()->getCommandQueue()->emplace<RendCtxCommand::SetScreenSize>(v);
	//	});
	//	config->setConfiguration<std::string>("debuggerDevelopperName", "Modify MyConfigurationFile.conf with your name", [&e](const std::string &name)
	//	{
	//		e->getInstance<PerformanceDebugger>()->setDevelopperName(name);
	//	});
	//
	//
	//	config->loadFile();
	//
	//	// We wait here for context initialization return
	//	bool contextInitReturnValue = contextInit.get();
	//	if (contextInitReturnValue == false)
	//		return EXIT_FAILURE;
	//
	//	// add main scene
	//	e->addScene(std::make_shared<BenchmarkScene>(e), "BenchmarkScene");
	//
	//	// bind scene
	//	if (!e->initScene("BenchmarkScene"))
	//		return (EXIT_FAILURE);
	//
	//	e->enableScene("BenchmarkScene", 100);
	//
	//#ifdef USE_IMGUI
	//	e->getRenderThread()->getTaskQueue()->emplaceFuture<AGE::TQC::BoolFunction, bool>([=](){
	//		AGE::Imgui::getInstance()->init(e.get());
	//		return true;
	//	}).get();
	//#endif
	//
	//	// launch engine
	//	e->getCurrentThreadCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait);
	//	e->_hasFrameBefore = true;
	//	e->getCommandQueue()->releaseReadability(TMQ::ReleasableQueue::WaitType::NoWait);
	//	e->first = true;
	//	e->getRenderThread()->last = true;
	//	while (e->commandQueueUpdate())
	//		;
	//	config->saveToFile();
	//	e->_release();

	//while (true)
	//{
	//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//}
	return (EXIT_SUCCESS);
}
