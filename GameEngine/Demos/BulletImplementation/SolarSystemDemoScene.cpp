#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "SolarSystemDemoScene.hh"

#include "Components/RotationForce.hpp"
#include <Components/CameraComponent.hpp>
#include <Components/TrackBallComponent.hpp>
#include <Components/AudioListener.hpp>
#include <Components/AudioEmitter.hpp>
#include <OpenGL/ComputeShader.hh>
#include <Systems/RotationForceSystem.hpp>
#include <Systems/MeshRenderSystem.h>
#include <Systems/CameraSystem.hpp>
#include <Systems/TrackBallSystem.hpp>
#include <Systems/AudioSystem.hpp>
#include <Audio/AudioManager.hh>
#include <Core/Engine.hh>

#include <MediaFiles/AssetsManager.hpp>

#include <SDL\SDL.h>

SolarSystemDemoScene::SolarSystemDemoScene(std::weak_ptr<Engine> engine) : AScene(engine)
{
}

SolarSystemDemoScene::~SolarSystemDemoScene(void)
{
}

Entity	SolarSystemDemoScene::createPlanet(float rotSpeed, float orbitSpeed,
												glm::vec3 &pos, glm::vec3 &scale,
												std::string const &shader,
												std::string const &tex1, std::string const &tex2, std::string const &tex3,
												std::string const &tex4)
{
	auto p = createEntity();
	auto e = createEntity();
	auto t = e->getLocalTransform();
	t = glm::translate(t, pos);
	t = glm::scale(t, scale);
	e->setLocalTransform(t);

	// EXAMPLE: HOW TO CREATE A MEDIA FILE DYNAMICALY
	//auto ballMesh = getInstance<AssetsManager>()->get<ObjFile>("obj__ball");
	//auto planetMesh = getInstance<AssetsManager>()->create<ObjFile>(tex1 + tex2 + tex3 + tex4, ballMesh);
	//planetMesh->material = getInstance<AssetsManager>()->create<MaterialFile>("material_" + tex1 + tex2 + tex3 + tex4, ballMesh->material);
	//planetMesh->material->materials[0].ambientTex = getInstance<AssetsManager>()->get<TextureFile>(tex1);
	//planetMesh->material->materials[0].diffuseTex = getInstance<AssetsManager>()->get<TextureFile>(tex2);
	//planetMesh->material->materials[0].specularTex = getInstance<AssetsManager>()->get<TextureFile>(tex3);
	//planetMesh->material->materials[0].normalTex = getInstance<AssetsManager>()->get<TextureFile>(tex4);
	//planetMesh->saveToFile();
	//planetMesh->material->saveToFile();

	
	auto planetMesh = getInstance<AssetsManager>()->loadFromFile<cereal::BinaryInputArchive>(File("./Assets/Serialized/" + tex1/* + tex2 + tex3 + tex4*/ + ".cpd"));

	std::shared_ptr<Component::MeshRenderer>	r = e->addComponent<Component::MeshRenderer>(planetMesh);

	r->setShader(shader);

	e->addComponent<Component::RotationForce>(glm::vec3(0, orbitSpeed, 0));
	p->addChild(e);
	p->addComponent<Component::RotationForce>(glm::vec3(0, rotSpeed, 0));
	return (p);
}

bool 			SolarSystemDemoScene::userStart()
{	
	rct<Component::CameraComponent>()
		.rct<Component::MeshRenderer>()
		.rct<Component::RotationForce>()
		.rct<Component::TrackBall>()
		.rct<Component::AudioListener>()
		.rct<Component::AudioEmitter>();

	// System Tests
	//
	//

	addSystem<RotationForceSystem>(0);
	addSystem<MeshRendererSystem>(0);
	addSystem<TrackBallSystem>(150);
	addSystem<AudioSystem>(170);
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

	auto s = getInstance<Renderer>()->addShader("MaterialBasic",
		"./Shaders/MaterialBasic.vp",
		"./Shaders/MaterialBasic.fp");

		getInstance<Renderer>()->addUniform("MaterialBasic")
			->init(s, "MaterialBasic", materialBasic);
		getInstance<Renderer>()->addUniform("PerFrame")
			->init(s, "PerFrame", perFrameVars);
		getInstance<Renderer>()->addUniform("PerModel")
			->init(s, "PerModel", perModelVars);

	getInstance<Renderer>()->addShader("earth", "./Shaders/earth.vp", "./Shaders/earth.fp");
	getInstance<Renderer>()->addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/tesselation.gp");
	getInstance<Renderer>()->addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
	getInstance<Renderer>()->addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");
	getInstance<Renderer>()->addShader("fboToScreen", "Shaders/fboToScreen.vp", "Shaders/fboToScreen.fp");
	getInstance<Renderer>()->addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
	getInstance<Renderer>()->addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");
	getInstance<Renderer>()->getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	getInstance<Renderer>()->getShader("basic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	getInstance<Renderer>()->getShader("basicLight")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	getInstance<Renderer>()->getShader("bump")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(2).build();
	getInstance<Renderer>()->getShader("fboToScreen")->addTarget(GL_COLOR_ATTACHMENT0)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	getInstance<Renderer>()->getShader("earth")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	getInstance<Renderer>()->getShader("brightnessFilter")->addTarget(GL_COLOR_ATTACHMENT1)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	getInstance<Renderer>()->getShader("blurY")->addTarget(GL_COLOR_ATTACHMENT2)
		.addLayer(GL_COLOR_ATTACHMENT0).addLayer(GL_COLOR_ATTACHMENT1).build();

	getInstance<Renderer>()->getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));

	getInstance<Renderer>()->bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("basicLight", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("basicLight", "MaterialBasic", "MaterialBasic");
	getInstance<Renderer>()->bindShaderToUniform("basic", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("basic", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("basic", "MaterialBasic", "MaterialBasic");
	getInstance<Renderer>()->bindShaderToUniform("earth", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("earth", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("earth", "MaterialBasic", "MaterialBasic");
	getInstance<Renderer>()->bindShaderToUniform("bump", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("bump", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("bump", "MaterialBasic", "MaterialBasic");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

	std::string		vars[] = 
	{
		"projection",
		"view"
	};

	auto sky = getInstance<Renderer>()->addShader("cubemapShader", "Shaders/cubemap.vp", "Shaders/cubemap.fp");

	getInstance<Renderer>()->getShader("cubemapShader")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();

	getInstance<Renderer>()->addUniform("cameraUniform")
		->init(sky, "cameraUniform", vars);

	getInstance<Renderer>()->bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");


	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__ball.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__Space.cpd"));
	auto music = getInstance<AudioManager>()->loadStream(File("./Assets/isolee.mp3"), Audio::AudioSpatialType::AUDIO_3D);


	// SERIALIZATION
	//File saveFile("SolarSystem.scenesave");
	//if (saveFile.exists())
	//{
	//	std::ifstream fileStream("SolarSystem.scenesave", std::ios_base::binary);
	//	load<cereal::JSONInputArchive>(fileStream);
	//	fileStream.close();
	//	return true;
	//}

	auto sun = createPlanet(0, 0, glm::vec3(0), glm::vec3(100), "basic", "texture__SunTexture");
	auto earth = createPlanet(7, 20, glm::vec3(300, 0, 0), glm::vec3(20),
		"earth",
		"texture__EarthTexture",
		"texture__EarthNightTexture",
		"texture__EarthClouds",
		"texture__EarthTextureBump");
	auto moon = createPlanet(0, 10, glm::vec3(5, 0, 0), glm::vec3(0.5), "bump", "texture__MoonTexture", "texture__MoonTextureBump");
	auto audioCpt = sun->addComponent<Component::AudioEmitter>();
	audioCpt->setAudio(music, "ambiant", CHANNEL_GROUP_MUSIC);
	audioCpt->play("ambiant", true);
	audioCpt->clearAudio("ambiant");
	audioCpt->setAudio(music, "ambiant", CHANNEL_GROUP_MUSIC);
	audioCpt->play("ambiant", true);
	earth->getChildsBegin()->get()->addChild(moon);


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
				sun->addChild(planets[i]);
			else
				planets[i - 1]->addChild(planets[i]);
			planets[i]->getComponent<Component::RotationForce>()->force = glm::vec3(10.0f);
		}
	}


	//
	//
	// END : Generating a lot of planet for performance test

	// --
	// Setting camera with skybox
	// --

	auto camera = createEntity();
	auto cameraComponent = camera->addComponent<Component::CameraComponent>();
	camera->setLocalTransform(glm::translate(camera->getLocalTransform(), glm::vec3(30000, 0, 30000)));
	auto trackBall = camera->addComponent<Component::TrackBall>(*(earth->getChildsBegin()), 50.0f, 3.0f, 1.0f);
	cameraComponent->attachSkybox("skybox__space", "cubemapShader");
	camera->addComponent<Component::AudioListener>();

	return (true);
}

bool 			SolarSystemDemoScene::userUpdate(double time)
{
	if (getInstance<Input>()->getInput(SDLK_l))
	{
		getInstance<SceneManager>()->enableScene("BulletDemo", 0);
		getInstance<SceneManager>()->disableScene("SolarSystemDemo");
	}
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
	{
		 //SERIALIZATION
		{
			std::ofstream s("SolarSystem.scenesave");
			save<cereal::JSONOutputArchive>(s);
			s.close();
		}
		return (false);
	}
	return (true);
}
