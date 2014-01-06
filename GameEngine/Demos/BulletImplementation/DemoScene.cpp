#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"

#include "ResourceManager/SharedMesh.hh"
#include "ResourceManager/Texture.hh"
#include "ResourceManager/CubeMap.hh"
#include "ResourceManager/ResourceManager.hh"

#include <Components/RotationForce.hh>
#include <Components/MaterialComponent.h>
#include <Components/CameraComponent.hh>
#include <Components/RigidBody.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>

#include <OpenGL/ComputeShader.hh>
#include <OpenGL/Attribute.hh>
#include <OpenGL/include/SDL/SDL_opengl.h>

#include <Systems/RotationForceSystem.hpp>
#include <Systems/GraphNodeSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/MeshRenderSystem.h>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/FPControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>

#include <Core/Engine.hh>

#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

Handle  DemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
	auto mesh = e->addComponent<Component::MeshRenderer>("model:ball");
	e->addComponent<Component::GraphNode>();
	auto prout = mesh->getMaterials();
	mesh->getMaterials()[0].ambientTex = _engine.getInstance<Resources::ResourceManager>().getResource(tex);
	mesh->setShader("MaterialBasic");
	return e;
}

Handle  DemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(1, 0, 0));
	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(0, 1, 0));
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	auto mesh = e->addComponent<Component::MeshRenderer>("model:cube");
	e->addComponent<Component::GraphNode>();
	mesh->getMaterials()[0].ambientTex = _engine.getInstance<Resources::ResourceManager>().getResource(tex);
	mesh->setShader("MaterialBasic");
	return e;
}

Handle  DemoScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::MESH, "model:monkey");
	auto mesh = e->addComponent<Component::MeshRenderer>("model:monkey");
	mesh->getMaterials()[0].ambientTex = _engine.getInstance<Resources::ResourceManager>().getResource(tex);
	e->addComponent<Component::GraphNode>();
	mesh->setShader("MaterialBasic");
	return e;
}

bool 			DemoScene::userStart()
{	
	std::srand(0);

	// System Tests
	//
	//
	addSystem<MeshRendererSystem>(0);
	addSystem<GraphNodeSystem>(0); // UPDATE ENTITIES TRANSFORMATION
	addSystem<BulletDynamicSystem>(10); // UPDATE PHYSIC WORLD
	addSystem<CollisionAdder>(20); // ADD COLLISION COMPONENT TO COLLIDING ENTITIES
	addSystem<FPControllerSystem>(50); // UPDATE FIRST PERSON CONTROLLER
	addSystem<FirstPersonViewSystem>(150); // UPDATE FIRST PERSON CAMERA
	addSystem<CameraSystem>(200); // UPDATE CAMERA AND RENDER TO SCREEN
	addSystem<CollisionCleaner>(300); // REMOVE COLLISION COMPONENTS FROM COLLIDING ENTITIES
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

	_engine.getInstance<Renderer>().addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/basic.gp");
	_engine.getInstance<Renderer>().addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
	_engine.getInstance<Renderer>().addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");
	_engine.getInstance<Renderer>().addShader("earth", "Shaders/earth.vp", "Shaders/earth.fp");
	_engine.getInstance<Renderer>().addShader("fboToScreen", "Shaders/fboToScreen.vp", "Shaders/fboToScreen.fp");
	_engine.getInstance<Renderer>().addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
	_engine.getInstance<Renderer>().addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");

	_engine.getInstance<Renderer>().getShader("basic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	_engine.getInstance<Renderer>().getShader("basicLight")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	_engine.getInstance<Renderer>().getShader("bump")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(2).build();
	_engine.getInstance<Renderer>().getShader("fboToScreen")->addTarget(GL_COLOR_ATTACHMENT0)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	_engine.getInstance<Renderer>().getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
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



	_engine.getInstance<Resources::ResourceManager>().addResource("model:ball", new Resources::SharedMesh(), "./Assets/ball/ball.obj");
	_engine.getInstance<Resources::ResourceManager>().addResource("model:cube", new Resources::SharedMesh(), "./Assets/cube/cube.obj");
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

	auto p1 = createCube(glm::vec3(0, 0, 0), glm::vec3(100, 1, 100), "texture:moon", 0.0f);
	//p1->getComponent<Component::RigidBody>()->setTransformConstraint(false, false, false);
	//p1->getComponent<Component::RigidBody>()->setRotationConstraint(false, false, false);

	{
		auto e = createEntity();
		e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 45.0f, glm::vec3(0, 1, 0));
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(10,10,10));
		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(10,10,10));

		auto rigidBody = e->addComponent<Component::RigidBody>(1.0f);
		rigidBody->setCollisionShape(Component::RigidBody::BOX);
		auto mesh = e->addComponent<Component::MeshRenderer>("model:cube");
		mesh->getMaterials()[0].ambientTex = _engine.getInstance<Resources::ResourceManager>().getResource("texture:moon");
		mesh->setShader("MaterialBasic");
		e->addComponent<Component::GraphNode>();
		e->getComponent<Component::RigidBody>()->setTransformConstraint(false, false, false);
		e->getComponent<Component::RigidBody>()->setRotationConstraint(true, true, true);
	}

	{
		auto e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(-5,0,0));
		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(6));

		auto rigidBody = e->addComponent<Component::RigidBody>(0);
		rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
		auto mesh = e->addComponent<Component::MeshRenderer>("model:ball");
		mesh->getMaterials()[0].ambientTex = _engine.getInstance<Resources::ResourceManager>().getResource("texture:moon");
		e->addComponent<Component::GraphNode>();
		mesh->setShader("MaterialBasic");
	}

	Handle character;
	{
		auto e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0,20,0));
		e->addComponent<Component::FPController>();
		e->addComponent<Component::GraphNode>();
		character = e;
	}

	Handle c1;
	for (unsigned int i = 0; i < 70; ++i)
	{
		if (i % 3)
		{
			c1 = createCube(glm::vec3(-3 + 0.2 * (float)i, 3 * i + 16, 0), glm::vec3(2, 1, 3), "texture:sun", 1.f);
		}
		else if (i % 2)
		{
			c1 = createSphere(glm::vec3(-3 + 0.2 * (float)i, 3 * i + 16, 0), glm::vec3(1, 1, 1), "texture:earth", 1.0f);
		}
		else
		{
			c1 = createMonkey(glm::vec3(-3 + 0.2 * (float)i, 3 * i + 16, 0), glm::vec3(std::rand() % 100 / 80.0f),"texture:earth", 1.0f);
			c1->setLocalTransform() = glm::rotate(c1->getLocalTransform(), std::rand() % 100 / 10.0f, glm::vec3(1, 1, 1));
		}
	}

	// --
	// Setting camera with skybox
	// --

	//setCamera(new TrackBall(_engine, follow, 30.0f, 1.0f, 1.0f));
	auto cameraComponent = character->addComponent<Component::CameraComponent>();
	character->addComponent<Component::FirstPersonView>();

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

	cameraComponent->attachSkybox("cubemap:space", "cubemapShader");
	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
