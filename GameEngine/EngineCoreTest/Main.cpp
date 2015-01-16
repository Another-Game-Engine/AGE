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
#include <Core/AssetsManager.hpp>

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
#include <Core/Tasks/Basics.hpp>
////////////////////////////////////////

#include <chrono>

using namespace AGE;

<<<<<<< HEAD
bool loadAssets(AGE::Engine *e)
{
#if 0
	return AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=](){
		e->getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/AGE-Assets-For-Test/Serialized/");
#ifdef RENDERING_ACTIVATED
		e->getInstance<AGE::AssetsManager>()->loadMesh(File("cube/cube.sage")/*, { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }*/);
		// LOAD MATERIAL TO FIX
		//e->getInstance<AGE::AssetsManager>()->loadMaterial(File("cube/cube.mage"));
		e->getInstance<AGE::AssetsManager>()->loadMesh(File("ball/ball.sage")/*, { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }*/);
		// LOAD MATERIAL TO FIX
		//e->getInstance<AGE::AssetsManager>()->loadMaterial(File("ball/ball.mage"));
		e->getInstance<AGE::AssetsManager>()->loadMesh(File("catwoman/catwoman.sage")/*, { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }*/);
		// LOAD MATERIAL TO FIX
		//e->getInstance<AGE::AssetsManager>()->loadMaterial(File("catwoman/catwoman.mage"));
		e->getInstance<AGE::AssetsManager>()->loadSkeleton(File("catwoman/catwoman.skage"));
		e->getInstance<AGE::AssetsManager>()->loadAnimation(File("catwoman/catwoman-roulade.aage"));

		e->getInstance<AGE::AssetsManager>()->loadMesh(File("sponza/sponza.sage")/*, { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }*/);

		// LOAD MATERIAL TO FIX
		// e->getInstance<AGE::AssetsManager>()->loadMaterial(File("sponza/sponza.mage"));
		return true;
#endif
	}).get();
#endif
	return (true);
}

=======
>>>>>>> master
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
