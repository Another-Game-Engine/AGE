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
#include <ResourceManager/ResourceManager.hh>
#include <Core/Renderer.hh>
#include <Managers/BulletCollisionManager.hpp>
#include "InGameScene.hpp"
#include "SecondScene.hpp"
#include <Utils/MetaData.hpp>

#include <Components/GraphNode.hpp>
#include <Utils/SmartPointer.hh>

DEFINE_META_POD( int );
DEFINE_META_POD( float );
DEFINE_META_POD( unsigned int );
DEFINE_META_POD( std::string );

DEFINE_META(Handle)
{
	ADD_MEMBER(_id);
	ADD_MEMBER(_manager);
}

//META_REG(Component::GraphNode);
//META_REG(Component::CameraComponent);

int			main(int ac, char **av)
{
	std::cout << META_TYPE(Handle)->getName() << " " << META_TYPE(Handle)->getSize() << std::endl;
	std::cout << META_TYPE(Component::TrackingCamera)->getName() << " " << META_TYPE(Component::TrackingCamera)->getSize() << std::endl;
	auto lol = META_TYPE(Handle);

	std::auto_ptr<Handle> h();
	auto smart = META_OBJECT(h);

	//std::shared_ptr<Handle> i();
	//auto smart2 = META_OBJECT(i);

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
//	e.setInstance<BulletCollisionManager>().init();
	e.setInstance<BulletDynamicManager, BulletCollisionManager>().init();

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
