#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"

//#include "ResourceManager/SharedMesh.hh
//#include "ResourceManager/CubeMap.hh"
//#include "ResourceManager/ResourceManager.hh"

#include <Managers/AssetsConvertorManager.hh>
#include <Managers/Fontconvertor.hh>
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
#include <Systems/CameraSystem.hpp>

#include <Core/Engine.hh>

#include <SDL\SDL.h>

DemoScene::DemoScene(std::weak_ptr<Engine> engine) : AScene(engine)
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
	addSystem<CameraSystem>(200); // UPDATE CAMERA AND RENDER TO SCREEN


	// end System Test

		auto convertor = getInstance<AssetsConvertorManager>();
		convertor->setOutputDirectory("../../Assets/Serialized/");
		convertor->setInputDirectory("../../Assets/");


		//convertor->load("../../Assets/cube/cube.obj");
		//convertor->serializeData("cube");
		//convertor->clear();

		//convertor->load("../../Assets/ball/ball.obj");
		//convertor->serializeData("ball");
		//convertor->clear();

		//convertor->load("../../Assets/torus.obj");
		//convertor->serializeData("torus");
		//convertor->clear();

		//convertor->load("../../Assets/crytek-sponza/sponza.obj");
		//convertor->serializeData("sponza");
		//convertor->clear();
		
		//convertor->load("../../Assets/Slides.tga");
		//convertor->load("../../Assets/Powerpoint.tga");
		//convertor->load("../../Assets/PowerpointTroll.tga");
		//convertor->serializeData("Slides");
		//convertor->clear();

		//convertor->load("../../Assets/EarthCloud.tga");
		//convertor->load("../../Assets/EarthNightTexture.tga");
		//convertor->load("../../Assets/EarthTexture.tga");
		//convertor->load("../../Assets/EarthTextureBump.tga");
		//convertor->load("../../Assets/EarthClouds.tga");
		//convertor->load("../../Assets/MoonNormalMap.tga");
		//convertor->load("../../Assets/MoonTexture.tga");
		//convertor->load("../../Assets/SunTexture.tga");
		//convertor->load("../../Assets/space.skybox");
		//convertor->serializeData("Space");
		//convertor->clear();

		//convertor->load("../../Assets/museum.obj");
		//convertor->serializeData("Museum");
		//convertor->clear();

		//convertor->load("../../Assets/conference-room.obj");
		//convertor->serializeData("Conference");
		//convertor->clear();

		//convertor->load("../../Assets/elf/elf.obj");
		//convertor->serializeData("Elf");
		//convertor->clear();

		//convertor->load("../../Assets/sketch-test.obj");
		//convertor->serializeData("SketchTest");
		//convertor->clear();

//		getInstance<FontConvertor>()->convertFont(File("./Assets/Montez-Regular.ttf"), { 5, 10, 20, 30, 40, 50, 60, 70 }, "./", "myFont");
//		getInstance<FontConvertor>()->convertFont(File("../../Assets/ContrailOne.ttf"), { 5, 10, 20, 30, 40, 50, 60, 70 }, "../../Assets/Serialized/", "myFont");

		//convertor->load("../../Assets/cat/cat.obj");
		//convertor->serializeData("Cat");
		//convertor->clear();

		//convertor->load("../../Assets/house.obj");
		//convertor->serializeData("House");
		//convertor->clear();

		//convertor->load("../../Assets/shoot.tga");
		//convertor->serializeData("shoot");
		//convertor->clear();

		//convertor->load("../../Assets/Trololo.tga");
		//convertor->serializeData("Trololo");
		//convertor->clear();

		//convertor->load("../../Assets/demoMuseum.obj");
		//convertor->serializeData("demoMuseum");
		//convertor->clear();

		//convertor->load("../../Assets/Bear_Brown/Bear_Brown.obj");
		//convertor->serializeData("bear");
		//convertor->clear();

		//convertor->load("../../Assets/dragon.obj");
		//convertor->serializeData("dragon");
		//convertor->clear();

		//convertor->load("../../Assets/TEXTS/boring-room.obj");
		//convertor->load("../../Assets/TEXTS/click-to-shoot.obj");
		//convertor->load("../../Assets/TEXTS/component.obj");
		//convertor->load("../../Assets/TEXTS/entity.obj");
		//convertor->load("../../Assets/TEXTS/sorry.obj");
		//convertor->load("../../Assets/TEXTS/system.obj");
		//convertor->load("../../Assets/TEXTS/welcome.obj");
		//convertor->load("../../Assets/TEXTS/why-age.obj");
		//convertor->load("../../Assets/TEXTS/asteroid-text.obj");
		//convertor->load("../../Assets/TEXTS/solarSystem-text.obj");
		//convertor->load("../../Assets/TEXTS/sponza-text-3d.obj");
		//convertor->load("../../Assets/TEXTS/credits.obj");
		//convertor->load("../../Assets/TEXTS/inception.obj");
		//convertor->load("../../Assets/TEXTS/thanks.obj");
		//convertor->load("../../Assets/TEXTS/using.obj");
		//convertor->load("../../Assets/TEXTS/roar.obj");

		//convertor->serializeData("3Dtexts");
		//convertor->clear();

		//convertor->load("../../Assets/starship/starship.obj");
		//convertor->serializeData("starship");
		//convertor->clear();

		//convertor->load("../../Assets/coffee/coffee.obj");
		//convertor->serializeData("coffee");
		//convertor->clear();

		convertor->load("../../Assets/Buttons.tga");
		convertor->serializeData("Buttons");
		convertor->clear();

		//convertor->load("../../Assets/fboasteroid.tga");
		//convertor->load("../../Assets/fbosolar.tga");
		//convertor->load("../../Assets/fbosponza.tga");

		//convertor->serializeData("fbotexture");
		//convertor->clear();

	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
		return (false);
	return (true);
}
