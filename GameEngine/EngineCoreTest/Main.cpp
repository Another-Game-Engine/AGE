#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>
#include <stdint.h>

#include <stdlib.h>
#include <Core/Engine.hh>
#include <Core/SceneManager.hh>
#include <Utils/PubSub.hpp>

#include <Render/GeometryManager.hh>
#include <Render/MaterialManager.hh>
#include <Render/RenderManager.hh>
// SCENES
#include "Scenes/BenchmarkScene.hh"

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Timer.hh>
#include <Utils/PubSub.hpp>
#include <Utils/PerformanceDebugger.hh>
#include <Core/AssetsManager.hpp>

#include <Systems/CameraSystem.hh> // just for the define... to rm for the future

//CONFIGS
#include <CONFIGS.hh>

bool loadAssets(std::shared_ptr<Engine> e)
{

	e->getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/AGE-Assets-For-Test/Serialized/");
#ifdef RENDERING_ACTIVATED
	e->getInstance<AGE::AssetsManager>()->loadMesh(File("cube/cube.sage"));
	e->getInstance<AGE::AssetsManager>()->loadMesh(File("ball/ball.sage"));
	auto t = e->getInstance<AGE::AssetsManager>()->loadMaterial(File("ball/ball.mage"));
#endif
#ifdef COMPLEX_MESH
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__cube.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__ball.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__dragon.cpd"));
#endif

	return true;
}

int			main(int ac, char **av)
{
	std::shared_ptr<Engine>	e = std::make_shared<Engine>();

	// Set Configurations
	auto config = e->setInstance<ConfigurationManager>(File("MyConfigurationFile.conf"));

	e->setInstance<PubSub::Manager>();
	e->setInstance<SdlContext, IRenderContext>();
	e->setInstance<Input>();
	e->setInstance<Timer>();
	e->setInstance<SceneManager>();
	e->setInstance<AGE::AssetsManager>();
	e->setInstance<PerformanceDebugger>("Developper Name");

#ifdef PHYSIC_SIMULATION
	e->setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
#endif

	// init engine
	if (e->init(0, 800, 600, "~AGE~ V0.0 Demo") == false)
		return (EXIT_FAILURE);

	// Set default window size
	// If config file has different value, it'll be changed automaticaly
	config->setConfiguration<glm::uvec2>("windowSize", glm::uvec2(800, 600), [&e](const glm::uvec2 &v)
	{
		e->getInstance<IRenderContext>()->setScreenSize(std::move(v));
	});
	config->setConfiguration<std::string>("debuggerDevelopperName", "Modify MyConfigurationFile.conf with your name", [&e](const std::string &name)
	{
		e->getInstance<PerformanceDebugger>()->setDevelopperName(name);
	});


	config->loadFile();

#ifdef RENDERING_ACTIVATED
	auto &geo = e->setInstance<gl::RenderManager>()->geometryManager;
	geo.addIndexPool();
	geo.addVertexPool();
	GLenum typeComponent[2] = { GL_FLOAT, GL_FLOAT };
	uint8_t sizeTypeComponent[2] = { sizeof(float), sizeof(float) };
	uint8_t nbrComponent[2] = { 2, 2 };
	geo.addVertexPool(2, typeComponent, sizeTypeComponent, nbrComponent);

	if (!loadAssets(e))
		return EXIT_FAILURE;
#endif

	// add main scene
	e->getInstance<SceneManager>()->addScene(std::make_shared<BenchmarkScene>(e), "BenchmarkScene");

	// bind scene
	if (!e->getInstance<SceneManager>()->initScene("BenchmarkScene"))
		return (EXIT_FAILURE);

	e->getInstance<SceneManager>()->enableScene("BenchmarkScene", 100);

	// launch engine
	if (e->start() == false)
		return (EXIT_FAILURE);
	while (e->update())
		;
	config->saveToFile();
	e->stop();
	return (EXIT_SUCCESS);
}
