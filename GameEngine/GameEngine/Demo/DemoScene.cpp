#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "BasicCam.hh"

#include "ResourceManager/SharedMesh.hh"
#include "ResourceManager/Texture.hh"
#include "ResourceManager/CubeMap.hh"
#include "Components/EmptyComponent.hh"
#include "Components/RotationForce.hh"

#include <SDL\SDL.h>

DemoScene::DemoScene(void)
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

	SmartPointer<Components::MeshRenderer>	r = new Components::MeshRenderer("renderer:" + tex1, "model:ball");

	SmartPointer<Material> materialPlanet = GameEngine::instance()->renderer().getMaterialManager().createMaterial("material:planet_" + shader);

	GameEngine::instance()->renderer().addShader("scndPassTest" + shader, "../GameEngine/Shaders/scndPassTest.vp", "../GameEngine/Shaders/scndPassTest.fp");

	materialPlanet->pushShader(shader).pushShader("scndPassTest" + shader);
	r->addMaterial(materialPlanet);

//	r->setShader(shader);
	r->addTexture(tex1, "texture1", 0);
	if (!tex2.empty())
		r->addTexture(tex2, "texture2", 1);
	if (!tex3.empty())
		r->addTexture(tex3, "texture3", 2);
	if (!tex4.empty())
		r->addTexture(tex4, "texture4", 2);
	e->addComponent(r);
	e->addComponent(new Components::RotationForce(glm::vec3(0, orbitSpeed, 0)));
	p->addSon(e);
	p->addComponent(new Components::RotationForce(glm::vec3(0, rotSpeed, 0)));
	return (p);
}

bool 			DemoScene::userStart()
{	
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

	OpenGLTools::Shader &s = GameEngine::instance()->renderer().addShader("earth",
																		  "../GameEngine/Shaders/earth.vp",
																		  "../GameEngine/Shaders/earth.fp")
		.bindActiveTexture("fDayTexture", 0)
		.bindActiveTexture("fNightTexture", 1)
		.bindActiveTexture("fClouds", 2)
		.bindActiveTexture("fBump", 3);

	GameEngine::instance()->renderer().addUniform("PerFrame")
		.init(&s, "PerFrame", perFrameVars);
	GameEngine::instance()->renderer().addUniform("PerModel")
		.init(&s, "PerModel", perModelVars);

	GameEngine::instance()->renderer().addShader("basic", "../GameEngine/Shaders/basic.vp", "../GameEngine/Shaders/basic.fp", "../GameEngine/Shaders/tesselation.gp");
	GameEngine::instance()->renderer().addShader("basicLight", "../GameEngine/Shaders/light.vp", "../GameEngine/Shaders/light.fp");
	GameEngine::instance()->renderer().addShader("bump", "../GameEngine/Shaders/bump.vp", "../GameEngine/Shaders/bump.fp")
		.bindActiveTexture("fTexture", 0)
		.bindActiveTexture("fBump", 1);
	GameEngine::instance()->renderer().addShader("fboToScreen", "../GameEngine/Shaders/fboToScreen.vp", "../GameEngine/Shaders/fboToScreen.fp");

	GameEngine::instance()->renderer().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));

	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerModel", "PerModel");
	GameEngine::instance()->renderer().bindShaderToUniform("basic", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("basic", "PerModel", "PerModel");
	GameEngine::instance()->renderer().bindShaderToUniform("earth", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("earth", "PerModel", "PerModel");
	GameEngine::instance()->renderer().bindShaderToUniform("bump", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("bump", "PerModel", "PerModel");

	GameEngine::instance()->resources().addResource("model:ball", new Resources::SharedMesh(), "../Assets/ball.obj");

	SmartPointer<Resources::Texture>		toRepeat = new Resources::Texture();

	toRepeat->setWrapMode(GL_REPEAT);
	GameEngine::instance()->resources().addResource("texture:sun", new Resources::Texture(), "../Assets/SunTexture.tga");
	GameEngine::instance()->resources().addResource("texture:earth", new Resources::Texture(), "../Assets/EarthTexture.tga");
	GameEngine::instance()->resources().addResource("texture:earthBump", new Resources::Texture(), "../Assets/EarthTextureBump.tga");
	GameEngine::instance()->resources().addResource("texture:earthNight", new Resources::Texture(), "../Assets/EarthNightTexture.tga");
	GameEngine::instance()->resources().addResource("texture:earthClouds", toRepeat, "../Assets/EarthClouds.tga");
	GameEngine::instance()->resources().addResource("texture:sun", new Resources::Texture(), "../Assets/SunTexture.tga");
	GameEngine::instance()->resources().addResource("texture:moon", new Resources::Texture(), "../Assets/MoonTexture.tga");
	GameEngine::instance()->resources().addResource("texture:moonBump", new Resources::Texture(), "../Assets/MoonNormalMap.tga");

	GameEngine::instance()->resources().addResource("cubemap:space", new Resources::CubeMap(), "../Assets/skyboxSpace");

	SmartPointer<Entity> sun = createPlanet(0, 0, glm::vec3(0), glm::vec3(100), "basic", "texture:sun");
	SmartPointer<Entity> earth = createPlanet(7, 20, glm::vec3(300, 0, 0), glm::vec3(20), "earth", "texture:earth", "texture:earthNight", "texture:earthClouds", "texture:earthBump");
	SmartPointer<Entity> moon = createPlanet(0, 10, glm::vec3(5, 0, 0), glm::vec3(0.5), "bump", "texture:moon", "texture:moonBump");

	getRoot()->addSon(earth);
	earth->getSonsBegin()->second->addSon(moon);
	getRoot()->addSon(sun);

	// --
	// Setting camera with skybox
	// --

	setCamera(new BasicCam(earth->getSonsBegin()->second));

	std::string		vars[] = 
	{
		"projection",
		"view"
	};

	OpenGLTools::Shader &sky = GameEngine::instance()->renderer().addShader("cubemapShader", "../GameEngine/Shaders/cubemap.vp", "../GameEngine/Shaders/cubemap.fp");

	GameEngine::instance()->renderer().addUniform("cameraUniform").
		init(&sky, "cameraUniform", vars);

	GameEngine::instance()->renderer().bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");

	getCamera()->attachSkybox("cubemap:space", "cubemapShader");



	// --
	// Framebuffer
	// --

	//GameEngine::instance()->renderer().getFbo().addLayer(0);

	GameEngine::instance()->renderer().getMaterialManager().compile(GameEngine::instance()->renderer().getFbo());

	return (true);
}

bool 			DemoScene::userUpdate()
{
	Engine		&engine = *GameEngine::instance();

	if (engine.inputs().getInput(SDLK_ESCAPE) ||
		engine.inputs().getInput(SDL_QUIT))
		return (false);
	return (true);
}
