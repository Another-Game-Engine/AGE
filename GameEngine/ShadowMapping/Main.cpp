// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include "Core/Engine.hh"
#include "Core/SceneManager.hh"
#include "Core/Renderer.hh"
#include "Utils/PubSub.hpp"
#include "Context/SdlContext.hh"

#include "DemoScene.hh"


static bool initInstanceEngine(Engine &engine)
{
	engine.setInstance<PubSub::Manager>();
	engine.setInstance<SdlContext, IRenderContext>();
	engine.setInstance<Input>();
	engine.setInstance<Timer>();
	engine.setInstance<Resources::ResourceManager>(&engine);
	engine.setInstance<Renderer>(&engine);
	engine.setInstance<SceneManager>();
	if (!engine.init())
	{
		std::cerr << "Engine initialization fail." << std::endl;
		return (false);
	}
	return (true);
}

static bool initSceneEngine(Engine &engine, std::string const &nameScene)
{
	engine.getInstance<SceneManager>().addScene(new DemoScene(engine), nameScene);
	if (!engine.getInstance<SceneManager>().initScene(nameScene))
	{
		std::cerr << "the init scene : " << nameScene << " fail." << std::endl;
		return (false);
	}
	engine.getInstance<SceneManager>().enableScene(nameScene, 0);
	return (true);
}

int main(int argc, char **argv)
{
	Engine engine;

	if (!initInstanceEngine(engine))
		return (EXIT_FAILURE);
	if (!initSceneEngine(engine, "demo-Shadow-Mapping"))
		return (EXIT_FAILURE);
	if (!engine.start())
		return (EXIT_FAILURE);
	while (engine.update());
	engine.stop();
	return (EXIT_SUCCESS);
}