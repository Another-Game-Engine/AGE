// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <Context/SdlContext.hh>
#include <Core/SceneManager.hh>
#include <ResourceManager/ResourceManager.hh>
#include <Core/Renderer.hh>
#include <Managers/BulletCollisionManager.hpp>
#include "InGameScene.hpp"
#include "SecondScene.hpp"

int			main(int ac, char **av)
{
	Engine	e;

	// set Rendering context of the engine
	// you can also set any other dependencies
	e.setInstance<PubSub::Manager>();
	e.setInstance<SdlContext, IRenderContext>();
	e.setInstance<Input>();
	e.setInstance<Timer>();
	e.setInstance<Resources::ResourceManager>(&e);
	e.setInstance<Renderer>(&e);
	e.setInstance<SceneManager>();
	e.setInstance<BulletCollisionManager>().init();

	// init engine
	if (e.init() == false)
		return (EXIT_FAILURE);

	// add scene
	e.getInstance<SceneManager>().addScene(new InGameScene(e), "InGameScene");

	// bind scene
	if (!e.getInstance<SceneManager>().initScene("InGameScene"))
		return false;
	e.getInstance<SceneManager>().enableScene("InGameScene", 100);

	// launch engine
	if (e.start() == false)
		return (EXIT_FAILURE);
	while (e.update())
		;
	//e.stop();
	return (EXIT_SUCCESS);
}
