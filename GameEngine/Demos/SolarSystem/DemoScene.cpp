#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "Core/Renderer.hh"
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
#include <Systems/GraphNodeSystem.hpp>
#include "ResourceManager/ResourceManager.hh"
#include <Core/Engine.hh>
#include <Entities/EntityManager.h>

#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

Handle	DemoScene::createPlanet(float rotSpeed, float orbitSpeed,
												glm::vec3 &pos, glm::vec3 &scale,
												std::string const &shader,
												std::string const &tex1, std::string const &tex2, std::string const &tex3,
												std::string const &tex4)
{
	auto &m = _engine.getInstance<EntityManager>();
	auto p = m.createEntity();
	auto e = m.createEntity();
	p->addComponent<Component::GraphNode>();
	e->addComponent<Component::GraphNode>();

	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);

	SmartPointer<Component::MeshRenderer>	r = e->addComponent<Component::MeshRenderer>("model:ball");

	SmartPointer<Material> materialPlanet = _engine.getInstance<Renderer>().getMaterialManager().createMaterial("material:planet_" + shader);

	materialPlanet->pushShader(shader);

	e->addComponent<Component::ComponentMaterial>(std::string("material:planet_" + shader));

	//r->addMaterial(materialPlanet);
	r->addTexture(tex1, 0);
	if (!tex2.empty())
		r->addTexture(tex2, 1);
	if (!tex3.empty())
		r->addTexture(tex3, 2);
	if (!tex4.empty())
		r->addTexture(tex4, 3);
	
	e->addComponent<Component::RotationForce>(glm::vec3(0, orbitSpeed, 0));
	p->getComponent<Component::GraphNode>()->addSon(e);
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
	addSystem<GraphNodeSystem>(2);

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

	OpenGLTools::Shader &s = _engine.getInstance<Renderer>().addShader("earth",
		"./Shaders/earth.vp",
		"./Shaders/earth.fp");
		//.bindActiveTexture("fDayTexture", 0)
		//.bindActiveTexture("fNightTexture", 1)
		//.bindActiveTexture("fClouds", 2)
		//.bindActiveTexture("fBump", 3);

	_engine.getInstance<Renderer>().addUniform("PerFrame")
		.init(&s, "PerFrame", perFrameVars);
	_engine.getInstance<Renderer>().addUniform("PerModel")
		.init(&s, "PerModel", perModelVars);

	_engine.getInstance<Renderer>().addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/tesselation.gp");
	_engine.getInstance<Renderer>().addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
	_engine.getInstance<Renderer>().addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");
		//.bindActiveTexture("fTexture", 0)
		//.bindActiveTexture("fBump", 1);
	_engine.getInstance<Renderer>().addShader("fboToScreen", "Shaders/fboToScreen.vp", "Shaders/fboToScreen.fp");
	_engine.getInstance<Renderer>().addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
	_engine.getInstance<Renderer>().addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");

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
	_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("earth", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("earth", "PerModel", "PerModel");
	_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerFrame", "PerFrame");
	_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerModel", "PerModel");

	_engine.getInstance<Resources::ResourceManager>().addResource("model:ball", new Resources::SharedMesh(), "./Assets/ball.obj");

	SmartPointer<Resources::Texture>		toRepeat = new Resources::Texture();

	toRepeat->setWrapMode(GL_REPEAT);
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:sun", new Resources::Texture(), "./Assets/SunTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earth", new Resources::Texture(), "./Assets/EarthTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthBump", new Resources::Texture(), "./Assets/EarthTextureBump.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthNight", new Resources::Texture(), "./Assets/EarthNightTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthClouds", toRepeat, "./Assets/EarthClouds.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:sun", new Resources::Texture(), "./Assets/SunTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:moon", new Resources::Texture(), "./Assets/MoonTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:moonBump", new Resources::Texture(), "./Assets/MoonNormalMap.tga");

	_engine.getInstance<Resources::ResourceManager>().addResource("cubemap:space", new Resources::CubeMap(), "./Assets/skyboxSpace");

	auto sun = createPlanet(0, 0, glm::vec3(0), glm::vec3(100), "basic", "texture:sun");
	auto earth = createPlanet(7, 20, glm::vec3(300, 0, 0), glm::vec3(20), "earth", "texture:earth", "texture:earthNight", "texture:earthClouds", "texture:earthBump");
	auto moon = createPlanet(0, 10, glm::vec3(5, 0, 0), glm::vec3(0.5), "bump", "texture:moon", "texture:moonBump");

	auto ttt = earth.get();
	auto iii = ttt->getComponent<Component::GraphNode>();
	auto jjj = iii->getSonsBegin();

	earth->getComponent<Component::GraphNode>()->getSonsBegin()->get()->getComponent<Component::GraphNode>()->addSon(moon);
//---> canceled to replace by
	//getRoot()->addSon(earth);
	//earth->getSonsBegin()->second->addSon(moon);
	//getRoot()->addSon(sun);

	// Generating a lot of planet for performance test
	//
	//


	// !!!! THIS CODE CAUSE A STACKOVERFLOW !!!
	//{
	//	unsigned int nbPlanet = 265;
	//	SmartPointer<Entity> planets[265];

	//	for (unsigned int i = 0; i < nbPlanet; ++i)
	//	{
	//		planets[i] = createPlanet((std::rand() % 200) / 100.0f
	//			, (std::rand() % 200) / 100.0f,
	//			glm::vec3(std::rand() % 300 - 150, std::rand() % 300 - 150, std::rand() % 300 - 150),
	//			glm::vec3(std::rand() % 10 + 10), "basic", "texture:sun");
	//		if (i == 0)
	//			sun->addSon(planets[i]);
	//		else
	//			planets[i - 1]->addSon(planets[i]);
	//	}
	//}
	{
		unsigned int nbPlanet = 350;
		Handle past;
		for (unsigned int i = 0; i < nbPlanet; ++i)
		{
			if (!past.get())
			{
				past = createPlanet((std::rand() % 200) / 100.0f
					, (std::rand() % 200) / 100.0f,
					glm::vec3(std::rand() % 300 - 150, std::rand() % 300 - 150, std::rand() % 300 - 150),
					glm::vec3(std::rand() % 10 + 10), "basic", "texture:sun");
				sun->getComponent<Component::GraphNode>()->addSon(past);
			}
			else
			{
				Handle p = createPlanet((std::rand() % 200) / 100.0f
					, (std::rand() % 200) / 100.0f,
					glm::vec3(std::rand() % 300 - 150, std::rand() % 300 - 150, std::rand() % 300 - 150),
					glm::vec3(std::rand() % 10 + 10), "basic", "texture:sun");
				past->getComponent<Component::GraphNode>()->addSon(p);
				past = p;
			}
		}
	}


	//
	//
	// END : Generating a lot of planet for performance test

	// --
	// Setting camera with skybox
	// --

	setCamera(new TrackBall(_engine, *(earth->getComponent<Component::GraphNode>()->getSonsBegin()), 50, 3, 1));
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

	getCamera()->attachSkybox("cubemap:space", "cubemapShader");

	//OpenGLTools::ComputeShader *cs = new OpenGLTools::ComputeShader();
	//cs->init(File("../GameEngine/Shaders/test.cp"));
	//cs->use();
	//glDispatchCompute(512/16, 512/16, 1);

	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
