#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"

//#include "ResourceManager/SharedMesh.hh
//#include "ResourceManager/CubeMap.hh"
//#include "ResourceManager/ResourceManager.hh"

#include <Managers/AssetsConvertorManager.hh>
#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>

#include <Components/RotationForce.hpp>
#include <Components/CameraComponent.hpp>

#include <OpenGL/ComputeShader.hh>
#include <OpenGL/Attribute.hh>
#include <OpenGL/include/SDL/SDL_opengl.h>

#include <Systems/RotationForceSystem.hpp>
#include <Systems/GraphNodeSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/MeshRenderSystem.h>

#include <Core/Engine.hh>

#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

bool 			DemoScene::userStart()
{	
	std::srand(0);

	// System Tests
	//
	//
	addSystem<MeshRendererSystem>(0);
	addSystem<GraphNodeSystem>(0); // UPDATE ENTITIES TRANSFORMATION
	addSystem<CameraSystem>(200); // UPDATE CAMERA AND RENDER TO SCREEN


	// end System Test

		auto &convertor = _engine.getInstance<AssetsConvertorManager>();
		convertor.setOutputDirectory("./Assets/Serialized/");

		//convertor.load("./Assets/cube/cube.obj");
		//convertor.serializeData("cube");
		//convertor.clear();

		//convertor.load("./Assets/ball/ball.obj");
		//convertor.serializeData("ball");
		//convertor.clear();

		//convertor.load("./Assets/galileo/galileo.obj");
		//convertor.serializeData("galileo");
		//convertor.clear();

		//convertor.load("./Assets/crytek-sponza/sponza.obj");
		//convertor.serializeData("sponza");
		//convertor.clear();

		//convertor.load("./Assets/EarthCloud.tga");
		//convertor.load("./Assets/EarthNightTexture.tga");
		//convertor.load("./Assets/EarthTexture.tga");
		//convertor.load("./Assets/EarthTextureBump.tga");
		//convertor.load("./Assets/EarthClouds.tga");
		//convertor.load("./Assets/MoonNormalMap.tga");
		//convertor.load("./Assets/MoonTexture.tga");
		//convertor.load("./Assets/SunTexture.tga");
		//convertor.load("./Assets/space.skybox");
		//convertor.serializeData("Space");
		//convertor.clear();

		convertor.load("./Assets/museum.obj");
		convertor.serializeData("Museum");
		convertor.clear();

		//convertor.load("./Assets/city/city.obj");
		//convertor.serializeData("City");
		//convertor.clear();

	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
