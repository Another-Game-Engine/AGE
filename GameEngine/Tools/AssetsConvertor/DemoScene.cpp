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

#include <Components/RotationForce.hh>
#include <Components/CameraComponent.hh>

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

		auto success = convertor.load("./Assets/cube/cube.obj");
		convertor.serializeData("cube");
		convertor.clear();

		convertor.load("./Assets/ball/ball.obj");
		convertor.serializeData("ball");
		convertor.clear();

		success = convertor.load("./Assets/galileo/galileo.obj");
		convertor.serializeData("galileo");
		convertor.clear();

		convertor.load("./Assets/crytek-sponza/sponza.obj");
		convertor.serializeData("sponza");
		convertor.clear();

		convertor.load("./Assets/EarthCloud.tga");
		convertor.load("./Assets/EarthNightTexture.tga");
		convertor.load("./Assets/EarthTexture.tga");
		convertor.load("./Assets/EarthTextureBump.tga");
		convertor.load("./Assets/EarthClouds.tga");
		convertor.load("./Assets/MoonNormalMap.tga");
		convertor.load("./Assets/MoonTexture.tga");
		convertor.load("./Assets/SunTexture.tga");
		convertor.load("./Assets/space.skybox");
		convertor.serializeData("Space");
		convertor.clear();

		//AMediaFile::loadFromList("./Assets/Serialized/export__cube.cpd");
		//AMediaFile::loadFromList("./Assets/Serialized/export__ball.cpd");
		//AMediaFile::loadFromList("./Assets/Serialized/export__galileo.cpd");
		//AMediaFile::loadFromList("./Assets/Serialized/export__sponza.cpd");
		//auto test = convertor.get(std::string("obj__sponza"));
		//std::cout << "::" << std::endl;


		std::string		perModelVars[] =
		{
			"model"
		};

		std::string		perFrameVars[] =
		{
			"projection",
			"view",
			"light",
			"time"
		};

		std::string		materialBasic[] =
		{
			"ambient",
			"diffuse",
			"specular",
			"transmittance",
			"emission",
			"shininess"
		};


		OpenGLTools::Shader &s = _engine.getInstance<Renderer>().addShader("MaterialBasic",
			"./Shaders/MaterialBasic.vp",
			"./Shaders/MaterialBasic.fp");

		_engine.getInstance<Renderer>().addUniform("MaterialBasic")
			.init(&s, "MaterialBasic", materialBasic);
		_engine.getInstance<Renderer>().addUniform("PerFrame")
			.init(&s, "PerFrame", perFrameVars);
		_engine.getInstance<Renderer>().addUniform("PerModel")
			.init(&s, "PerModel", perModelVars);

		_engine.getInstance<Renderer>().getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
		_engine.getInstance<Renderer>().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");


		//_engine.getInstance<Resources::ResourceManager>().addResource("cubemap:space", new Resources::CubeMap(), "./Assets/lake.skybox");

		std::string		vars[] =
		{
			"projection",
			"view"
		};

		OpenGLTools::Shader &sky = _engine.getInstance<Renderer>().addShader("cubemapShader", "Shaders/cubemap.vp", "Shaders/cubemap.fp");
		_engine.getInstance<Renderer>().getShader("cubemapShader")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
		_engine.getInstance<Renderer>().addUniform("cameraUniform").
			init(&sky, "cameraUniform", vars);
		_engine.getInstance<Renderer>().bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");

		Entity floor;

		Entity e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0, -10, 0));
		e->addComponent<Component::GraphNode>();
		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(100, 100, 100));
		auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::loadFromFile<cereal::BinaryInputArchive>("./Assets/Serialized/obj__sponza.cpd"));
		mesh->setShader("MaterialBasic");
		floor = e;



	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
