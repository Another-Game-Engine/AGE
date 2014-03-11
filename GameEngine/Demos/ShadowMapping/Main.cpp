// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

# include <stdlib.h>
# include <crtdbg.h>

# include <iostream>
# include "Core/Engine.hh"
# include "Core/SceneManager.hh"
# include "Core/Renderer.hh"
# include <MediaFiles/AssetsManager.hpp>
# include "Utils/PubSub.hpp"
# include "Context/SdlContext.hh"
# include "DemoScene.hh"
# include <OpenGL/VertexManager.hh>


static bool initInstanceEngine(std::shared_ptr<Engine> engine)
{
	engine->setInstance<PubSub::Manager>();
	engine->setInstance<SdlContext, IRenderContext>();
	engine->setInstance<Input>();
	engine->setInstance<Timer>();
	engine->setInstance<AssetsManager>()->init();
	engine->setInstance<Renderer>();
	engine->setInstance<SceneManager>();
	if (!engine->init())
	{
		std::cerr << "Engine initialization fail." << std::endl;
		return (false);
	}
	std::array<Attribute, 4> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 2),
	};

	engine->setInstance<VertexManager<4>>(param)->init();
	return (true);
}

static bool initSceneEngine(std::shared_ptr<Engine> engine, std::string const &nameScene)
{
	engine->getInstance<SceneManager>()->addScene(std::make_shared<DemoScene>(engine), nameScene);
	if (!engine->getInstance<SceneManager>()->initScene(nameScene))
	{
		std::cerr << "the init scene : " << nameScene << " fail." << std::endl;
		return (false);
	}
	engine->getInstance<SceneManager>()->enableScene(nameScene, 0);
	return (true);
}

int main(int argc, char **argv)
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();

	if (!initInstanceEngine(engine))
		return (EXIT_FAILURE);
	if (!initSceneEngine(engine, "demo-Shadow-Mapping"))
		return (EXIT_FAILURE);
	if (!engine->start())
		return (EXIT_FAILURE);
	while (engine->update());
	engine->stop();
	return (EXIT_SUCCESS);
}