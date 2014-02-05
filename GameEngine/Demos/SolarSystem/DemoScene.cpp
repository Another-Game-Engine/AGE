#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"

#include "Components/RotationForce.hh"
#include <Components/CameraComponent.hh>
#include <Components/TrackBallComponent.hpp>
#include <OpenGL/ComputeShader.hh>
#include <Systems/RotationForceSystem.hpp>
#include <Systems/MeshRenderSystem.h>
#include <Systems/GraphNodeSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/TrackBallSystem.hpp>
#include <Audio/AudioManager.hpp>
#include <Core/Engine.hh>

#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

Entity	DemoScene::createPlanet(float rotSpeed, float orbitSpeed,
												glm::vec3 &pos, glm::vec3 &scale,
												std::string const &shader,
												std::string const &tex1, std::string const &tex2, std::string const &tex3,
												std::string const &tex4)
{
	auto p = createEntity();
	auto e = createEntity();
	p->addComponent<Component::GraphNode>();
	e->addComponent<Component::GraphNode>();

	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);

	// EXAMPLE: HOW TO CREATE A MEDIA FILE DYNAMICALY
	auto ballMesh = AMediaFile::get<ObjFile>("obj__ball");
	auto planetMesh = AMediaFile::create<ObjFile>(tex1 + tex2 + tex3 + tex4, ballMesh);
	planetMesh->material = AMediaFile::create<MaterialFile>("", ballMesh->material);
	planetMesh->material->materials[0].ambientTex = AMediaFile::get<TextureFile>(tex1);
	planetMesh->material->materials[0].diffuseTex = AMediaFile::get<TextureFile>(tex2);
	planetMesh->material->materials[0].specularTex = AMediaFile::get<TextureFile>(tex3);
	planetMesh->material->materials[0].normalTex = AMediaFile::get<TextureFile>(tex4);

	std::shared_ptr<Component::MeshRenderer>	r = e->addComponent<Component::MeshRenderer>(planetMesh);

	r->setShader(shader);

	e->addComponent<Component::RotationForce>(glm::vec3(0, orbitSpeed, 0));
	p->getComponent<Component::GraphNode>()->addSon(e);
	p->addComponent<Component::RotationForce>(glm::vec3(0, rotSpeed, 0));
	return (p);
}

bool 			DemoScene::userStart()
{	
	rct<Component::CameraComponent>()
		.rct<Component::GraphNode>()
		.rct<Component::MeshRenderer>()
		.rct<Component::RotationForce>()
		.rct<Component::TrackBall>();

	// System Tests
	//
	//

	addSystem<RotationForceSystem>(0);
	addSystem<MeshRendererSystem>(0);
	addSystem<GraphNodeSystem>(100);
	addSystem<TrackBallSystem>(150);
	addSystem<CameraSystem>(200);

	//
	//
	// end System Test

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

	_engine.getInstance<Renderer>().addShader("earth", "./Shaders/earth.vp", "./Shaders/earth.fp");
	_engine.getInstance<Renderer>().addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/tesselation.gp");
	_engine.getInstance<Renderer>().addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
	_engine.getInstance<Renderer>().addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");
	_engine.getInstance<Renderer>().addShader("fboToScreen", "Shaders/fboToScreen.vp", "Shaders/fboToScreen.fp");
	_engine.getInstance<Renderer>().addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
	_engine.getInstance<Renderer>().addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");
	_engine.getInstance<Renderer>().getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	_engine.getInstance<Renderer>().getShader("basic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	_engine.getInstance<Renderer>().getShader("basicLight")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	_engine.getInstance<Renderer>().getShader("bump")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(2).build();
	_engine.getInstance<Renderer>().getShader("fboToScreen")->addTarget(GL_COLOR_ATTACHMENT0)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	_engine.getInstance<Renderer>().getShader("earth")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	_engine.getInstance<Renderer>().getShader("brightnessFilter")->addTarget(GL_COLOR_ATTACHMENT1)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	_engine.getInstance<Renderer>().getShader("blurY")->addTarget(GL_COLOR_ATTACHMENT2)
		.addLayer(GL_COLOR_ATTACHMENT0).addLayer(GL_COLOR_ATTACHMENT1).build();

	_engine.getInstance<Renderer>().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));

	_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "MaterialBasic", "MaterialBasic");
	_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("basic", "MaterialBasic", "MaterialBasic");
	_engine.getInstance<Renderer>().bindShaderToUniform("earth", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("earth", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("earth", "MaterialBasic", "MaterialBasic");
	_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("bump", "MaterialBasic", "MaterialBasic");
	_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

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


	// SERIALIZATION
	File saveFile("SolarSystem.scenesave");
	if (saveFile.exists())
	{
		std::ifstream fileStream("SolarSystem.scenesave", std::ios_base::binary);
		load<cereal::JSONInputArchive>(fileStream);
		fileStream.close();
		return true;
	}


	AMediaFile::loadFromList("./Assets/Serialized/export__ball.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__Space.cpd");
	auto music = _engine.getInstance<AudioManager>().loadStream(File("./Assets/isolee.mp3"), Audio::AudioSpatialType::AUDIO_2D);
	if (music)
	{
		music->play();
	}

	auto sun = createPlanet(0, 0, glm::vec3(0), glm::vec3(100), "basic", "texture__SunTexture");
	auto earth = createPlanet(7, 20, glm::vec3(300, 0, 0), glm::vec3(20),
		"earth",
		"texture__EarthTexture",
		"texture__EarthNightTexture",
		"texture__EarthClouds",
		"texture__EarthTextureBump");
	auto moon = createPlanet(0, 10, glm::vec3(5, 0, 0), glm::vec3(0.5), "bump", "texture__MoonTexture", "texture__MoonTextureBump");
	earth->getComponent<Component::GraphNode>()->getSonsBegin()->get()->getComponent<Component::GraphNode>()->addSon(moon);

	// Generating a lot of planet for performance test
	//
	//

	{
# define NBR_PLANET 10
		unsigned int nbPlanet = NBR_PLANET;
		Entity planets[NBR_PLANET];

		for (unsigned int i = 0; i < nbPlanet; ++i)
		{
			planets[i] = createPlanet((std::rand() % 200) / 100.0f
				, (std::rand() % 200) / 100.0f,
				glm::vec3(std::rand() % 300 - 150, std::rand() % 300 - 150, std::rand() % 300 - 150),
				glm::vec3(std::rand() % 10 + 10), "basic", "texture__SunTexture");
			if (i == 0)
				sun->getComponent<Component::GraphNode>()->addSon(planets[i]);
			else
				planets[i - 1]->getComponent<Component::GraphNode>()->addSon(planets[i]);
		}
	}


	//
	//
	// END : Generating a lot of planet for performance test

	// --
	// Setting camera with skybox
	// --

	auto camera = createEntity();
	camera->addComponent<Component::GraphNode>();
	auto cameraComponent = camera->addComponent<Component::CameraComponent>();
	auto trackBall = camera->addComponent<Component::TrackBall>(*(earth->getComponent<Component::GraphNode>()->getSonsBegin()), 50.0f, 3.0f, 1.0f);
	cameraComponent->attachSkybox("skybox__space", "cubemapShader");

	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
	{
		 //SERIALIZATION
		{
			//std::ofstream s("SolarSystem.scenesave");
			//save<cereal::JSONOutputArchive>(s);
			//s.close();
		}
		return (false);
	}
	return (true);
}
