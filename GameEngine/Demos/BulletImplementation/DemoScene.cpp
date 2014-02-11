#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"

#include <Components/RotationForce.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/RigidBody.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/AudioEmitter.hpp>
#include <Components/AudioListener.hpp>

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
#include <Systems/AudioSystem.hpp>
#include <BallSoundSystem.hpp>

#include <Core/Engine.hh>

#include <SDL\SDL.h>

#define BALL_TAG (0)

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

Entity  DemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::SPHERE);

	auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__ball"));
	e->addComponent<Component::GraphNode>();
	mesh->setShader("MaterialBasic");
	return e;
}

Entity  DemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(1, 0, 0));
	e->setLocalTransform() = glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(0, 1, 0));
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__cube"));
	e->addComponent<Component::GraphNode>();
	mesh->setShader("MaterialBasic");
	return e;
}

Entity  DemoScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_dynamic_galileo");
	auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__galileo"));
	mesh->setShader("MaterialBasic");
	e->addComponent<Component::GraphNode>();
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
	addSystem<BallSoundSystem>(220);
	addSystem<AudioSystem>(250);
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
			"time",
			"lightNbr"
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


	AMediaFile::loadFromList("./Assets/Serialized/export__cube.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__ball.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__Space.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__sponza.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__galileo.cpd");
//	AMediaFile::loadFromList("./Assets/Serialized/export__Museum.cpd");

	_engine.getInstance<AudioManager>().loadSound(File("./Assets/switch19.wav"), Audio::AudioSpatialType::AUDIO_3D);
	_engine.getInstance<AudioManager>().loadStream(File("./Assets/isolee.mp3"), Audio::AudioSpatialType::AUDIO_3D);
	_engine.getInstance<AudioManager>().loadSound(File("./Assets/arriveOnFloor.mp3"), Audio::AudioSpatialType::AUDIO_3D);
	_engine.getInstance<AudioManager>().loadSound(File("./Assets/jump.mp3"), Audio::AudioSpatialType::AUDIO_3D);

	// EXAMPLE: HOW TO CREATE A MEDIA FILE DYNAMICALY
	auto defaultBallMesh = AMediaFile::get<ObjFile>("obj__ball");
	auto planetMesh = AMediaFile::create<ObjFile>("my_planet", defaultBallMesh);
	planetMesh->material = AMediaFile::create<MaterialFile>("my_planet_material", defaultBallMesh->material);
	auto testsss = planetMesh->material->materials[0];
	planetMesh->material->materials[0].ambientTex = AMediaFile::get<TextureFile>("texture__EarthTexture");
	planetMesh->material->materials[0].diffuseTex = AMediaFile::get<TextureFile>("texture__EarthNightTexture");
	planetMesh->material->materials[0].specularTex = AMediaFile::get<TextureFile>("texture__EarthClouds");
	planetMesh->material->materials[0].normalTex = AMediaFile::get<TextureFile>("texture__EarthTextureBump");

	// EXAMPLE: HOW TO SAVE TO FILE A MEDIA FILE CREATED DYNAMICALY
	AMediaFile::saveToFile("my_planet_material", "./Assets/Serialized/");
	AMediaFile::saveToFile("my_planet", "./Assets/Serialized/");

	// EXAMPLE LOAD FROM SAVE
	AMediaFile::loadFromFile<cereal::BinaryInputArchive>(File("./Assets/Serialized/my_planet.cpd"));

	// CREATE SPONZA CHURCH
	{
		auto e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0));
		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(70));
//		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(70, 1, 70));
//		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(100));
		auto rigidBody = e->addComponent<Component::RigidBody>(0);
		rigidBody->setMass(0);
		rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sponza");
//		rigidBody->setCollisionShape(Component::RigidBody::BOX);
//		rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_museum");
		rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
		auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__sponza"));
//		auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__cube"));
//		auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__museum"));
		mesh->setShader("MaterialBasic");
		e->addComponent<Component::GraphNode>();
	}

	Entity character;
	std::shared_ptr<Component::CameraComponent> cameraComponent;

	{
		auto e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0,100,0));
		auto fpc = e->addComponent<Component::FPController>();
		e->addComponent<Component::GraphNode>();
		character = e;
		cameraComponent = character->addComponent<Component::CameraComponent>();
		character->addComponent<Component::FirstPersonView>();
		e->addComponent<Component::AudioListener>();
		auto ae = e->addComponent<Component::AudioEmitter>();
		auto arriveOnFloor = _engine.getInstance<AudioManager>().getAudio("arriveOnFloor");
		auto jump = _engine.getInstance<AudioManager>().getAudio("jump");
		ae->setAudio(arriveOnFloor, "arriveOnFloor", CHANNEL_GROUP_EFFECT);
		ae->setAudio(jump, "jump", CHANNEL_GROUP_EFFECT);
	}

	{
		auto e = createMonkey(glm::vec3(19, 0.9, -0.59), glm::vec3(1.5), "texture__SunTexture", 1.0f);
		auto rigidbody = e->getComponent<Component::RigidBody>();
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;
		auto audioCpt = e->addComponent<Component::AudioEmitter>();
		audioCpt->setAudio(_engine.getInstance<AudioManager>().getAudio("isolee"), "ambiant", CHANNEL_GROUP_MUSIC);
		audioCpt->play("ambiant", true);

	}

	// --
	// Setting camera with skybox
	// --

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

	cameraComponent->attachSkybox("skybox__space", "cubemapShader");
	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	static std::queue<Entity> stack;

	if (_engine.getInstance<Input>().getInput(SDL_BUTTON_RIGHT))
	{
		glm::vec3 from, to;
		getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
		auto test = _engine.getInstance<BulletCollisionManager>().rayCast(from, from + to * 1000.0f);
		if (test.size() != 0)
		{
			for (auto e : test)
			{
				if (e->isTagged(BALL_TAG))
					destroy(e);
			}
		}
	}
	static float delay = 0.0f;
	if (_engine.getInstance<Input>().getInput(SDL_BUTTON_LEFT) && delay <= 0.0f)
	{
		glm::vec3 from, to;
		getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
		auto e = createSphere(from + to * 1.5f, glm::vec3(0.2f), "on s'en bas la race", 1.0f);
		auto rigidbody = e->getComponent<Component::RigidBody>();
		rigidbody->getBody().applyCentralImpulse(convertGLMVectorToBullet(to * 10.0f));
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;
		e->addComponent<Component::AudioEmitter>()->setAudio(_engine.getInstance<AudioManager>().getAudio("switch19"), "collision", CHANNEL_GROUP_EFFECT);
		e->addTag(BALL_TAG);
		if (stack.size() > 300)
		{
			destroy(stack.front());
			stack.pop();
		}
		stack.push(e);
		delay = 0.1f;
	}
	if (delay >= 0.0f)
		delay -= time;
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
