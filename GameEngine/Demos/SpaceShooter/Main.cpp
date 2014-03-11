// for leak detection
//#define _CRTDBG_MAP_ALLOC
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
#include <Core/Renderer.hh>
#include <Managers/BulletCollisionManager.hpp>
#include <MediaFiles/AssetsManager.hpp>
#include "InGameScene.hpp"

int			main(int ac, char **av)
{
	std::shared_ptr<Engine>	e = std::make_shared<Engine>();

	// set Rendering context of the engine
	// you can also set any other dependencies
	e->setInstance<PubSub::Manager>();
	e->setInstance<SdlContext, IRenderContext>();
	e->setInstance<Input>();
	e->setInstance<Timer>();
	e->setInstance<AssetsManager>()->init();
	e->setInstance<Renderer>();
	e->setInstance<SceneManager>();
//	e.setInstance<BulletCollisionManager>().init();
	e->setInstance<BulletDynamicManager, BulletCollisionManager>()->init();

	// init engine
	if (e->init() == false)
		return (EXIT_FAILURE);

	std::array<Attribute, 4> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 2),
	};

	e->setInstance<VertexManager<4>>(param)->init();

	// add scene
	e->getInstance<SceneManager>()->addScene(std::make_shared<InGameScene>(e), "InGameScene");

	// bind scene
	if (!e->getInstance<SceneManager>()->initScene("InGameScene"))
		return false;
	e->getInstance<SceneManager>()->enableScene("InGameScene", 100);

	// launch engine
	if (e->start() == false)
		return (EXIT_FAILURE);
	while (e->update())
		;
	//e.stop();
	return (EXIT_SUCCESS);
}
