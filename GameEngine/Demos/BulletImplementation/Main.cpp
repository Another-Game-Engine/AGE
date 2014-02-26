// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include <Core/Engine.hh>
#include "BulletDemoScene.hh"
#include "SolarSystemDemoScene.hh"
#include <Utils/PubSub.hpp>
#include <Context/SdlContext.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Managers/BulletDynamicManager.hpp>
#include <MediaFiles/AssetsManager.hpp>
#include <Audio/AudioManager.hh>
#include <OpenGL/VertexManager.hh>

int			main(int ac, char **av)
{
	Engine	e;

	// set Rendering context of the engine
	// you can also set any other dependencies

	e.setInstance<PubSub::Manager>();
	e.setInstance<SdlContext, IRenderContext>();
	e.setInstance<Input>();
	e.setInstance<Timer>();
	e.setInstance<AssetsManager>()->init(&e);
	e.setInstance<Renderer>(&e);
	e.setInstance<SceneManager>();
	e.setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
	e.setInstance<AudioManager>()->init();

	// init engine
	if (e.init() == false)
		return (EXIT_FAILURE);

	std::array<Attribute, 4> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 2),
	};

	e.setInstance<VertexManager<4>>(param)->init();

	// add scene
	e.getInstance<SceneManager>()->addScene(new BulletDemoScene(e), "BulletDemo");
	e.getInstance<SceneManager>()->addScene(new SolarSystemDemoScene(e), "SolarSystemDemo");

	// bind scene
	if (!e.getInstance<SceneManager>()->initScene("BulletDemo"))
		return false;
	if (!e.getInstance<SceneManager>()->initScene("SolarSystemDemo"))
		return false;
	e.getInstance<SceneManager>()->enableScene("BulletDemo", 0);

	// lanch engine
	if (e.start() == false)
		return (EXIT_FAILURE);
	while (e.update())
		;
	//e.stop();
	return (EXIT_SUCCESS);
}
