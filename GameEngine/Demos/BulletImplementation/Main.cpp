// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "Utils/PubSub.hpp"
#include "Context/SdlContext.hh"
#include "Core/SceneManager.hh"
#include "ResourceManager/ResourceManager.hh"
#include "Core/Renderer.hh"
#include "Entities/EntityManager.h"

#include "BulletManager.hpp"

int			main(int ac, char **av)
{
	Engine	e;

	// set Rendering context of the engine
	// you can also set any other dependencies
	e.setInstance<PubSub::Manager>();
	e.setInstance<EntityManager>(&e);
	e.setInstance<SdlContext, IRenderContext>();
	e.setInstance<Input>();
	e.setInstance<Timer>();
	e.setInstance<Resources::ResourceManager>();
	e.setInstance<Renderer>();
	e.setInstance<SceneManager>();
	e.setInstance<BulletManager>().init();

	// init engine
	if (e.init() == false)
		return (EXIT_FAILURE);

	// add scene
	e.getInstance<SceneManager>().addScene(new DemoScene(e), "demo");

	// bind scene
	e.getInstance<SceneManager>().bindScene("demo");

	// lanch engine
	if (e.start() == false)
		return (EXIT_FAILURE);
	while (e.update())
		;
	//e.stop();
	return (EXIT_SUCCESS);
}
