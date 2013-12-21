#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"
#include "TrackBall.hh"

#include "ResourceManager/SharedMesh.hh"
#include "ResourceManager/Texture.hh"
#include "ResourceManager/CubeMap.hh"
#include "Components/RotationForce.hh"
#include "Components/MaterialComponent.h"
#include <Components/CameraComponent.hh>
#include <Component/RigidBody.hpp>
#include <OpenGL/ComputeShader.hh>
#include <Systems/RotationForceSystem.hpp>
#include <Systems/MeshRenderSystem.h>
#include <Systems/GraphNodeSystem.hpp>
#include <System/BulletSystem.hpp>
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

Handle  DemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &material, std::string const &tex, float mass)
{
	auto &m = _engine.getInstance<EntityManager>();
	auto e = m.createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
	auto mesh = e->addComponent<Component::MeshRenderer>("model:ball");
	auto mat = e->addComponent<Component::MaterialComponent>(material);
	mesh->addTexture(tex, 0);
	e->addComponent<Component::GraphNode>();
	return e;
}

Handle  DemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &material, std::string const &tex, float mass)
{
	auto &m = _engine.getInstance<EntityManager>();

	auto e = m.createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(1, 0, 0));
	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(0, 1, 0));
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	auto mesh = e->addComponent<Component::MeshRenderer>("model:cube");
	auto mat = e->addComponent<Component::MaterialComponent>(material);
	mesh->addTexture(tex, 0);
	e->addComponent<Component::GraphNode>();
	return e;
}

Handle  DemoScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &material, std::string const &tex, float mass)
{
	auto &m = _engine.getInstance<EntityManager>();

	auto e = m.createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::MESH, "model:monkey");
	auto mesh = e->addComponent<Component::MeshRenderer>("model:monkey");
	auto mat = e->addComponent<Component::MaterialComponent>(material);
	mesh->addTexture(tex, 0);
	e->addComponent<Component::GraphNode>();
	return e;
}

bool 			DemoScene::userStart()
{	
	std::srand(0);

	// System Tests
	//
	//
	addSystem<BulletSystem>(0);
	addSystem<GraphNodeSystem>(1);
	addSystem<MeshRendererSystem>(2);// ->setRenderDebugMode(true);

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

	_engine.getInstance<Renderer>().addUniform("PerFrame")
		.init(&s, "PerFrame", perFrameVars);
	_engine.getInstance<Renderer>().addUniform("PerModel")
		.init(&s, "PerModel", perModelVars);

	_engine.getInstance<Renderer>().addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/basic.gp");
	_engine.getInstance<Renderer>().addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
	_engine.getInstance<Renderer>().addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");

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
	_engine.getInstance<Resources::ResourceManager>().addResource("model:cube", new Resources::SharedMesh(), "./Assets/cube.obj");
	_engine.getInstance<Resources::ResourceManager>().addResource("model:square", new Resources::SharedMesh(), "./Assets/square.obj");
	_engine.getInstance<Resources::ResourceManager>().addResource("model:monkey", new Resources::SharedMesh(), "./Assets/monkey.obj");

	SmartPointer<Resources::Texture>		toRepeat = new Resources::Texture();

	toRepeat->setWrapMode(GL_REPEAT);
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:sun", new Resources::Texture(), "./Assets/SunTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earth", new Resources::Texture(), "./Assets/EarthTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthBump", new Resources::Texture(), "./Assets/EarthTextureBump.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthNight", new Resources::Texture(), "./Assets/EarthNightTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthClouds", toRepeat, "./Assets/EarthClouds.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:moon", new Resources::Texture(), "./Assets/MoonTexture.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("texture:moonBump", new Resources::Texture(), "./Assets/MoonNormalMap.tga");
	_engine.getInstance<Resources::ResourceManager>().addResource("cubemap:space", new Resources::CubeMap(), "./Assets/skyboxSpace");

	SmartPointer<Material> materialBasic = _engine.getInstance<Renderer>().getMaterialManager().createMaterial("material:basic");
	materialBasic->pushShader("basic");

	auto p1 = createCube(glm::vec3(0, 0, 0), glm::vec3(100, 1, 100), "material:basic", "texture:moon", 1.0f);
	p1->getComponent<Component::RigidBody>()->setTransformConstraint(false, false, false);
	p1->getComponent<Component::RigidBody>()->setRotationConstraint(false, false, false);
	//p1->setLocalTransform() = glm::scale(p1->getLocalTransform(), glm::vec3(0.5, 1, 0.5));
	//p1->getComponent<Component::RigidBody>()->updateScale();
//	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 15.0f, glm::vec3(0, 0, 1));

	Handle c1;
	for (unsigned int i = 0; i < 100; ++i)
	{
		if (i % 3)
		{
			c1 = createCube(glm::vec3(-3 + 0.2 * (float)i, 3 * i + 16, 0), glm::vec3(2, 1, 3), "material:basic", "texture:sun", 1.0f);
		}
		else if (i % 2)
		{
			c1 = createSphere(glm::vec3(-3 + 0.2 * (float)i, 3 * i + 16, 0), glm::vec3(1, 1, 1), "material:basic", "texture:earth", 1.0f);
		}
		else
		{
//			c1 = createMonkey(glm::vec3(-3 + 0.2 * (float)i, 3 * i + 16, 0), glm::vec3(std::rand() % 100 / 80.0f), "material:basic", "texture:earth", 10.0f);
//			c1->setLocalTransform() = glm::rotate(c1->getLocalTransform(), std::rand() % 100 / 10.0f, glm::vec3(1, 1, 1));
		}
	}

	// --
	// Setting camera with skybox
	// --

	setCamera(new TrackBall(_engine, p1, 30.0f, 1.0f, 1.0f));
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


	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
