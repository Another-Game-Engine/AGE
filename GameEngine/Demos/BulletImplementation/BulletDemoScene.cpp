#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "BulletDemoScene.hh"

#include <MediaFiles/AssetsManager.hpp>

#include <Components/RotationForce.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/RigidBody.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/AudioEmitter.hpp>
#include <Components/AudioListener.hpp>
#include <Components/SpriteComponent.hh>

#include <OpenGL/ComputeShader.hh>
#include <OpenGL/Attribute.hh>
#include <OpenGL/include/SDL/SDL_opengl.h>

#include <Systems/RotationForceSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/MeshRenderSystem.h>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/FPControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/AudioSystem.hpp>
#include <Systems/SpriteSystem.hh>
#include <BallSoundSystem.hpp>

#include <Text/FontManager.hh>
#include <Sprite/SpriteManager.hh>

#include <Core/Engine.hh>

#include <SDL\SDL.h>

#include <MyTags.hpp>

BulletDemoScene::BulletDemoScene(std::weak_ptr<Engine> engine) : AScene(engine)
{
}

BulletDemoScene::~BulletDemoScene(void)
{
}

Entity  BulletDemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	auto t = e->getLocalTransform();
	t = glm::translate(t, pos);
	t = glm::scale(t, scale);
	e->setLocalTransform(t);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::SPHERE);

	auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
	mesh->setShader("MaterialBasic");
	return e;
}

Entity  BulletDemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	auto t = e->getLocalTransform();
	t = glm::translate(t, pos);
	t = glm::rotate(t, 0.0f, glm::vec3(1, 0, 0));
	t = glm::rotate(t, 0.0f, glm::vec3(0, 1, 0));
	t = glm::scale(t, scale);
	e->setLocalTransform(t);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__cube"));
	mesh->setShader("MaterialBasic");
	return e;
}

Entity  BulletDemoScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	auto t = e->getLocalTransform();
	t = glm::translate(t, pos);
	t = glm::scale(t, scale);
	e->setLocalTransform(t);
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_dynamic_galileo");
	auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
	mesh->setShader("MaterialBasic");
	return e;
}

bool 			BulletDemoScene::userStart()
{
	std::srand(0);

	rct<Component::CameraComponent>()
		.rct<Component::MeshRenderer>()
		.rct<Component::RotationForce>()
		.rct<Component::AudioListener>()
		.rct<Component::AudioEmitter>()
		.rct<Component::RigidBody>()
		.rct<Component::Collision>()
		.rct<Component::FirstPersonView>()
		.rct<Component::FPController>();


	// System Tests
	//
	//
	addSystem<MeshRendererSystem>(0);
	addSystem<BulletDynamicSystem>(10); // UPDATE PHYSIC WORLD
	addSystem<CollisionAdder>(20); // ADD COLLISION COMPONENT TO COLLIDING ENTITIES
	addSystem<FPControllerSystem>(50); // UPDATE FIRST PERSON CONTROLLER
	addSystem<FirstPersonViewSystem>(150); // UPDATE FIRST PERSON CAMERA
	addSystem<CameraSystem>(200); // UPDATE CAMERA AND RENDER TO SCREEN
	addSystem<BallSoundSystem>(220);
	addSystem<AudioSystem>(250);
	addSystem<CollisionCleaner>(300); // REMOVE COLLISION COMPONENTS FROM COLLIDING ENTITIES
	addSystem<RotationForceSystem>(320);
	addSystem<SpriteSystem>(350); // DRAW SPRITES
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

	getInstance<Renderer>()->addShader("2DText",
		"./Shaders/2DText.vp",
		"./Shaders/2DText.fp");

	getInstance<Renderer>()->addShader("SpriteBasic",
		"./Shaders/SpriteBasic.vp",
		"./Shaders/SpriteBasic.fp");

	getInstance<Renderer>()->addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/basic.gp");
	getInstance<Renderer>()->addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
	getInstance<Renderer>()->addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");
	getInstance<Renderer>()->addShader("earth", "Shaders/earth.vp", "Shaders/earth.fp");
	getInstance<Renderer>()->addShader("fboToScreen", "Shaders/fboToScreen.vp", "Shaders/fboToScreen.fp");
	getInstance<Renderer>()->addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
	getInstance<Renderer>()->addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");

	getInstance<Renderer>()->getShader("basic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	getInstance<Renderer>()->getShader("basicLight")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
	getInstance<Renderer>()->getShader("bump")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(2).build();
	getInstance<Renderer>()->getShader("fboToScreen")->addTarget(GL_COLOR_ATTACHMENT0)
		.addLayer(GL_COLOR_ATTACHMENT0).build();
	getInstance<Renderer>()->getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	getInstance<Renderer>()->getShader("2DText")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
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


	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__cube.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__ball.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__Space.cpd"));
	//getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__sponza.cpd"));
	//	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__SketchTest.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__galileo.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__Museum.cpd"));

	getInstance<FontManager>()->loadFont(File("./Assets/Serialized/myFont.cpdFont"));

	getInstance<SpriteManager>()->loadFile(File("./Assets/Serialized/GreyMan.CPDAnimation"));

	getInstance<AudioManager>()->loadSound(File("./Assets/switch19.wav"), Audio::AudioSpatialType::AUDIO_3D);
	getInstance<AudioManager>()->loadStream(File("./Assets/isolee.mp3"), Audio::AudioSpatialType::AUDIO_3D);
	getInstance<AudioManager>()->loadSound(File("./Assets/arriveOnFloor.mp3"), Audio::AudioSpatialType::AUDIO_3D);
	getInstance<AudioManager>()->loadSound(File("./Assets/jump.mp3"), Audio::AudioSpatialType::AUDIO_3D);

	// EXAMPLE: HOW TO CREATE A MEDIA FILE DYNAMICALY
	auto defaultBallMesh = getInstance<AssetsManager>()->get<ObjFile>("obj__ball");
	auto planetMesh = getInstance<AssetsManager>()->create<ObjFile>("my_planet", defaultBallMesh);
	planetMesh->material = getInstance<AssetsManager>()->create<MaterialFile>("my_planet_material", defaultBallMesh->material);
	auto testsss = planetMesh->material->materials[0];
	planetMesh->material->materials[0].ambientTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthTexture");
	planetMesh->material->materials[0].diffuseTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthNightTexture");
	planetMesh->material->materials[0].specularTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthClouds");
	planetMesh->material->materials[0].normalTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthTextureBump");

	// EXAMPLE LOAD FROM SAVE
	getInstance<AssetsManager>()->loadFromFile<cereal::BinaryInputArchive>(File("./Assets/Serialized/my_planet.cpd"));

	// SKYBOX SETTINGS

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

	//File saveFile("BulletScene.scenesave");
	//if (saveFile.exists())
	//{
	//	std::ifstream fileStream("BulletScene.scenesave", std::ios_base::binary);
	//	load<cereal::BinaryInputArchive>(fileStream);
	//	fileStream.close();
	//	return true;
	//}

	// CREATE SPRITE ANIMATION
	{
		auto e = createEntity();
		e->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "idle"),
			getInstance<Renderer>()->getShader("SpriteBasic"));
		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0, 300, 0)));
		auto e2 = createEntity();
		e2->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "walk"),
			getInstance<Renderer>()->getShader("SpriteBasic"));
		e2->setLocalTransform(glm::translate(e2->getLocalTransform(), glm::vec3(1700, 0, 0)));
		auto e3 = createEntity();
		e3->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "crouch_idle"),
			getInstance<Renderer>()->getShader("SpriteBasic"));
		e3->setLocalTransform(glm::translate(e3->getLocalTransform(), glm::vec3(1700, 400, 0)));
		auto e4 = createEntity();
		e4->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "kitten"),
			getInstance<Renderer>()->getShader("SpriteBasic"));
		e4->setLocalTransform(glm::translate(e4->getLocalTransform(), glm::vec3(1700, 800, 0)));
		e4->setLocalTransform(glm::scale(e4->getLocalTransform(), glm::vec3(0.1)));
	}


	// CREATE SPONZA CHURCH
	{
		auto e = createEntity();
		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0)));
		e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(70)));
		//		e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(70, 1, 70));
		auto rigidBody = e->addComponent<Component::RigidBody>(0);
		rigidBody->setMass(0);
		//rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sponza");
		//rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sketch-test");
		//rigidBody->setCollisionShape(Component::RigidBody::BOX);
		rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_museum");
		rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
		rigidBody->getShape().setMargin(0.001f);
		rigidBody->getBody().setFriction(1.0f);
		rigidBody->getBody().setRestitution(0.9f);
		//		auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__sketch-test"));
		//		auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__cube"));
		//auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__sponza"));
		auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__museum"));
		mesh->setShader("MaterialBasic");
	}

	Entity character;
	std::shared_ptr<Component::CameraComponent> cameraComponent;

	{
		auto e = createEntity();
		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0, 100, 0)));
		auto fpc = e->addComponent<Component::FPController>();
		character = e;
		cameraComponent = character->addComponent<Component::CameraComponent>();
		character->addComponent<Component::FirstPersonView>();
		e->addComponent<Component::AudioListener>();
		auto ae = e->addComponent<Component::AudioEmitter>();
		auto arriveOnFloor = getInstance<AudioManager>()->getAudio("arriveOnFloor");
		auto jump = getInstance<AudioManager>()->getAudio("jump");
		ae->setAudio(arriveOnFloor, "arriveOnFloor", CHANNEL_GROUP_EFFECT);
		ae->setAudio(jump, "jump", CHANNEL_GROUP_EFFECT);
	}

	{
		auto e = createMonkey(glm::vec3(19, 0.9, -0.59), glm::vec3(1.5), "texture__SunTexture", 1.0f);
		auto rigidbody = e->getComponent<Component::RigidBody>();
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;
		auto audioCpt = e->addComponent<Component::AudioEmitter>();
		audioCpt->setAudio(getInstance<AudioManager>()->getAudio("isolee"), "ambiant", CHANNEL_GROUP_MUSIC);
		audioCpt->play("ambiant", true);

	}

	// --
	// Setting camera with skybox
	// --

	cameraComponent->attachSkybox("skybox__space", "cubemapShader");
	return (true);
}

bool 			BulletDemoScene::userUpdate(double time)
{
	static std::queue<Entity> stack;

	if (getInstance<Input>()->getInput(SDLK_l))
	{
		getInstance<SceneManager>()->enableScene("SolarSystemDemo", 0);
		getInstance<SceneManager>()->disableScene("BulletDemo");
	}

	if (getInstance<Input>()->getInput(SDL_BUTTON_RIGHT))
	{
		glm::vec3 from, to;
		getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
		auto test = getInstance<BulletCollisionManager>()->rayCast(from, from + to * 1000.0f);
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
	if (getInstance<Input>()->getInput(SDL_BUTTON_LEFT) && delay <= 0.0f)
	{
		glm::vec3 from, to;
		getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
		auto e = createSphere(from + to * 1.5f, glm::vec3(0.2f), "on s'en bas la race", 1.0f);
		auto rigidbody = e->getComponent<Component::RigidBody>();
		auto &body = rigidbody->getBody();
		body.applyCentralImpulse(convertGLMVectorToBullet(to * 10.0f));
		body.getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		body.getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;
		body.setFriction(1.0f);
		body.setRestitution(0.9f);
		e->addComponent<Component::AudioEmitter>()->setAudio(getInstance<AudioManager>()->getAudio("switch19"), "collision", CHANNEL_GROUP_EFFECT);
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
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
	{
		//SERIALIZATION
		{
			std::ofstream s("BulletScene.scenesave", std::ios_base::binary);
			save<cereal::BinaryOutputArchive>(s);
			s.close();
		}
		return (false);
	}
	static auto timeCounter = 0.0f;
	static auto frameCounter = 0;
	static auto lastFrame = 0;
	timeCounter += time;
	frameCounter += 1;
	getInstance<FontManager>()->draw2DString("FPS : " + std::to_string(lastFrame), "myFont", 40, glm::ivec2(10, 10), glm::vec4(1), "2DText");

	getInstance<FontManager>()->draw2DString("Entity Nbr : " + std::to_string(getNumberOfEntities()), "myFont", 30, glm::ivec2(10, 50), glm::vec4(1), "2DText");

	getInstance<FontManager>()->draw2DString("This is test 1", "myFont", 30, glm::ivec2(10, 100), glm::vec4(1,0,1,1), "2DText");
	getInstance<FontManager>()->draw2DString("This is test 2", "myFont", 35, glm::ivec2(10, 150), glm::vec4(0,1,1,1), "2DText");
	getInstance<FontManager>()->draw2DString("This is test 3", "myFont", 40, glm::ivec2(10, 200), glm::vec4(1,1,0,1), "2DText");
	getInstance<FontManager>()->draw2DString("This is test 4", "myFont", 45, glm::ivec2(10, 250), glm::vec4(0.5,0.2,0.4,1), "2DText");
	getInstance<FontManager>()->draw2DString("This is test 5", "myFont", 50, glm::ivec2(10, 300), glm::vec4(1,1,1,0.5), "2DText");

	if (timeCounter >= 1.0f)
	{
		lastFrame = frameCounter;
		timeCounter = 0.0f;
		frameCounter = 0;
	}

	return (true);
}
