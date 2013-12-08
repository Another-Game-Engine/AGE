#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "TrackBall.hh"

#include "ResourceManager/SharedMesh.hh"
#include "ResourceManager/Texture.hh"
#include "ResourceManager/CubeMap.hh"
#include "Components/EmptyComponent.hh"
#include "Components/RotationForce.hh"
#include "Components/MaterialComponent.h"
#include <OpenGL/ComputeShader.hh>
#include <Systems/RotationForceSystem.hpp>
#include <Systems/MeshRenderSystem.h>

#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

SmartPointer<Entity>	DemoScene::createPlanet(float rotSpeed, float orbitSpeed,
												glm::vec3 &pos, glm::vec3 &scale,
												std::string const &shader,
												std::string const &tex1, std::string const &tex2, std::string const &tex3,
												std::string const &tex4)
{
	SmartPointer<Entity>		p = new Entity;
	SmartPointer<Entity>		e = new Entity;

	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);

	SmartPointer<Component::MeshRenderer>	r = e->addComponent<Component::MeshRenderer>("renderer:" + tex1, "model:ball");

	SmartPointer<Material> materialPlanet = _engine.renderer().getMaterialManager().createMaterial("material:planet_" + shader);

	materialPlanet->pushShader(shader);

	e->addComponent<Component::ComponentMaterial>(std::string("material:planet_" + shader))->setMaterial("material:planet_" + shader);

	//r->addMaterial(materialPlanet);
	r->addTexture(tex1, 0);
	if (!tex2.empty())
		r->addTexture(tex2, 1);
	if (!tex3.empty())
		r->addTexture(tex3, 2);
	if (!tex4.empty())
		r->addTexture(tex4, 3);

	e->addComponent<Component::RotationForce>(glm::vec3(0, orbitSpeed, 0));
	p->addSon(e);
	p->addComponent<Component::RotationForce>(glm::vec3(0, rotSpeed, 0));
	return (p);
}

bool 			DemoScene::userStart()
{	
	// System Tests
	//
	//

	addSystem<RotationForceSystem>(0);
	addSystem<MeshRendererSystem>(1)->setRenderDebugMode(true);

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

	OpenGLTools::Shader &s = _engine.renderer().addShader("earth",
		"../GameEngine/Shaders/earth.vp",
		"../GameEngine/Shaders/earth.fp");
		//.bindActiveTexture("fDayTexture", 0)
		//.bindActiveTexture("fNightTexture", 1)
		//.bindActiveTexture("fClouds", 2)
		//.bindActiveTexture("fBump", 3);

	_engine.renderer().addUniform("PerFrame")
		.init(&s, "PerFrame", perFrameVars);
	_engine.renderer().addUniform("PerModel")
		.init(&s, "PerModel", perModelVars);

	_engine.renderer().addShader("basic", "../GameEngine/Shaders/basic.vp", "../GameEngine/Shaders/basic.fp", "../GameEngine/Shaders/tesselation.gp");
	_engine.renderer().addShader("basicLight", "../GameEngine/Shaders/light.vp", "../GameEngine/Shaders/light.fp");
	_engine.renderer().addShader("bump", "../GameEngine/Shaders/bump.vp", "../GameEngine/Shaders/bump.fp");
		//.bindActiveTexture("fTexture", 0)
		//.bindActiveTexture("fBump", 1);
	_engine.renderer().addShader("fboToScreen", "../GameEngine/Shaders/fboToScreen.vp", "../GameEngine/Shaders/fboToScreen.fp");
	_engine.renderer().addShader("brightnessFilter", "../GameEngine/Shaders/brightnessFilter.vp", "../GameEngine/Shaders/brightnessFilter.fp");
	_engine.renderer().addShader("blurY", "../GameEngine/Shaders/brightnessFilter.vp", "../GameEngine/Shaders/blur1.fp");

	_engine.renderer().getShader("basic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	_engine.renderer().getShader("basicLight")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	_engine.renderer().getShader("bump")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(2).build();
	_engine.renderer().getShader("fboToScreen")->addTarget(GL_COLOR_ATTACHMENT0)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	_engine.renderer().getShader("earth")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	_engine.renderer().getShader("brightnessFilter")->addTarget(GL_COLOR_ATTACHMENT1)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	_engine.renderer().getShader("blurY")->addTarget(GL_COLOR_ATTACHMENT2)
		.addLayer(GL_COLOR_ATTACHMENT0).addLayer(GL_COLOR_ATTACHMENT1).build();

	_engine.renderer().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));

	_engine.renderer().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
	_engine.renderer().bindShaderToUniform("basicLight", "PerModel", "PerModel");
	_engine.renderer().bindShaderToUniform("basic", "PerFrame", "PerFrame");
	_engine.renderer().bindShaderToUniform("basic", "PerModel", "PerModel");
	_engine.renderer().bindShaderToUniform("earth", "PerFrame", "PerFrame");
	_engine.renderer().bindShaderToUniform("earth", "PerModel", "PerModel");
	_engine.renderer().bindShaderToUniform("bump", "PerFrame", "PerFrame");
	_engine.renderer().bindShaderToUniform("bump", "PerModel", "PerModel");

	_engine.resources().addResource("model:ball", new Resources::SharedMesh(), "../Assets/ball.obj");

	SmartPointer<Resources::Texture>		toRepeat = new Resources::Texture();

	toRepeat->setWrapMode(GL_REPEAT);
	_engine.resources().addResource("texture:sun", new Resources::Texture(), "../Assets/SunTexture.tga");
	_engine.resources().addResource("texture:earth", new Resources::Texture(), "../Assets/EarthTexture.tga");
	_engine.resources().addResource("texture:earthBump", new Resources::Texture(), "../Assets/EarthTextureBump.tga");
	_engine.resources().addResource("texture:earthNight", new Resources::Texture(), "../Assets/EarthNightTexture.tga");
	_engine.resources().addResource("texture:earthClouds", toRepeat, "../Assets/EarthClouds.tga");
	_engine.resources().addResource("texture:sun", new Resources::Texture(), "../Assets/SunTexture.tga");
	_engine.resources().addResource("texture:moon", new Resources::Texture(), "../Assets/MoonTexture.tga");
	_engine.resources().addResource("texture:moonBump", new Resources::Texture(), "../Assets/MoonNormalMap.tga");

	_engine.resources().addResource("cubemap:space", new Resources::CubeMap(), "../Assets/skyboxSpace");

	SmartPointer<Entity> sun = createPlanet(0, 0, glm::vec3(0), glm::vec3(100), "basic", "texture:sun");
	SmartPointer<Entity> earth = createPlanet(7, 20, glm::vec3(300, 0, 0), glm::vec3(20), "earth", "texture:earth", "texture:earthNight", "texture:earthClouds", "texture:earthBump");
	SmartPointer<Entity> moon = createPlanet(0, 10, glm::vec3(5, 0, 0), glm::vec3(0.5), "bump", "texture:moon", "texture:moonBump");

	getRoot()->addSon(earth);
	earth->getSonsBegin()->second->addSon(moon);
	getRoot()->addSon(sun);

	// Generating a lot of planet for performance test
	//
	//

	unsigned int nbPlanet = 10;

	SmartPointer<Entity> planets[10];
	for (unsigned int i = 0; i < nbPlanet; ++i)
	{
		planets[i] = createPlanet((std::rand() % 200) / 100.0f
			, (std::rand() % 200) / 100.0f,
			glm::vec3(std::rand() % 300 - 150, std::rand() % 300 - 150, std::rand() % 300 - 150),
			glm::vec3(std::rand() % 30), "basic", "texture:sun");
		if (i == 0)
			sun->addSon(planets[i]);
		else
			planets[i - 1]->addSon(planets[i]);
	}

	//
	//
	// END : Generating a lot of planet for performance test

	// --
	// Setting camera with skybox
	// --

	setCamera(new TrackBall(earth->getSonsBegin()->second, 50, 3, 1));

	std::string		vars[] = 
	{
		"projection",
		"view"
	};

	OpenGLTools::Shader &sky = _engine.renderer().addShader("cubemapShader", "../GameEngine/Shaders/cubemap.vp", "../GameEngine/Shaders/cubemap.fp");

	_engine.renderer().getShader("cubemapShader")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();

	_engine.renderer().addUniform("cameraUniform").
		init(&sky, "cameraUniform", vars);

	_engine.renderer().bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");

	getCamera()->attachSkybox("cubemap:space", "cubemapShader");


	//OpenGLTools::ComputeShader *cs = new OpenGLTools::ComputeShader();
	//cs->init(File("../GameEngine/Shaders/test.cp"));
	//cs->use();
	//glDispatchCompute(512/16, 512/16, 1);

	return (true);
}

bool 			DemoScene::userUpdate()
{
	if (_engine.inputs().getInput(SDLK_ESCAPE) ||
		_engine.inputs().getInput(SDL_QUIT))
		return (false);
	return (true);
}
