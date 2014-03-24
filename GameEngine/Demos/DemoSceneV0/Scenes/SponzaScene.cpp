#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "SponzaScene.hh"

#include <Components/RotationForce.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/RigidBody.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/SpriteComponent.hh>

#include <OpenGL/ComputeShader.hh>
#include <OpenGL/Attribute.hh>
#include <OpenGL/include/SDL/SDL_opengl.h>

#include <Systems/RotationForceSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/FPControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/SpriteSystem.hh>
#include <Systems/DownSampleSystem.hh>
#include <Systems/PostFxSystem.hh>
#include <Systems/BlitFinalRender.hh>

#include <Text/FontManager.hh>
#include <Sprite/SpriteManager.hh>

#include <Core/Engine.hh>

#include <Systems\LightRenderingSystem.hh>

#include <SDL\SDL.h>

#include <MyTags.hpp>


Entity globalCamera;

SponzaScene::SponzaScene(std::weak_ptr<Engine> engine) : AScene(engine)
{
}

SponzaScene::~SponzaScene(void)
{
}

Entity SponzaScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform(glm::translate(e->getLocalTransform(), pos));
	e->setLocalTransform(glm::scale(e->getLocalTransform(), scale));
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::SPHERE);

	auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
	mesh->setShader("MaterialBasic");
	return e;
}

Entity SponzaScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform(glm::translate(e->getLocalTransform(), pos));
	e->setLocalTransform(glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(1, 0, 0)));
	e->setLocalTransform(glm::rotate(e->getLocalTransform(), 0.0f, glm::vec3(0, 1, 0)));
	e->setLocalTransform(glm::scale(e->getLocalTransform(), scale));
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__cube"));
	mesh->setShader("MaterialBasic");
	return e;
}

Entity SponzaScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass)
{
	auto e = createEntity();
	e->setLocalTransform(glm::translate(e->getLocalTransform(), pos));
	e->setLocalTransform(glm::scale(e->getLocalTransform(), scale));
	auto rigidBody = e->addComponent<Component::RigidBody>(mass);
	rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_dynamic_galileo");
	auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
	mesh->setShader("MaterialBasic");
	return e;
}

bool SponzaScene::userStart()
{
	std::srand(0);

	setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
	std::dynamic_pointer_cast<BulletDynamicManager>(getInstance<BulletCollisionManager>())->getWorld()->setGravity(btVector3(0, -10, 0));
	setInstance<FontManager>()->init();

	std::cout << "OPENGL VERSION : " << glGetString(GL_VERSION) << std::endl;

	getInstance<Renderer>()->addShader("fboToScreenMultisampled", "../../Shaders/fboToScreen.vp", "../../Shaders/fboToScreenMultisampled.fp");
	getInstance<Renderer>()->addShader("fboToScreen", "../../Shaders/fboToScreen.vp", "../../Shaders/fboToScreen.fp");

	auto s = getInstance<Renderer>()->addShader("MaterialBasic",
		"../../Shaders/MaterialBasic.vp",
		"../../Shaders/MaterialBasic.fp");

	auto shadowDepth = getInstance<Renderer>()->addShader("ShadowDepth" , "../../Shaders/ShadowMapping.vp", "../../Shaders/ShadowMapping.fp");

	// System Tests
	//
	//
	addSystem<BulletDynamicSystem>(0); // UPDATE PHYSIC WORLD
	addSystem<CollisionAdder>(10); // ADD COLLISION COMPONENT TO COLLIDING ENTITIES
	addSystem<FPControllerSystem>(20); // UPDATE FIRST PERSON CONTROLLER
	addSystem<FirstPersonViewSystem>(30); // UPDATE FIRST PERSON CAMERA
	addSystem<CollisionCleaner>(60); // REMOVE COLLISION COMPONENTS FROM COLLIDING ENTITIES

	addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	addSystem<LightRenderingSystem>(80); // Render with the lights
	addSystem<SpriteSystem>(90); // DRAW SPRITES
	addSystem<DownSampleSystem>(100); // DOWNSAMPLE FBO
	addSystem<PostFxSystem>(110); // POST FXs

	getSystem<PostFxSystem>()->setHDRIdealIllumination(0.3f);
	getSystem<PostFxSystem>()->setHDRAdaptationSpeed(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxLightDiminution(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxDarkImprovement(1.2f);
	getSystem<PostFxSystem>()->useHDR(false);
	getSystem<PostFxSystem>()->useBloom(false);

	//
	//
	// end System Test


	std::string		perModelVars[] =
	{
		"model"
	};

	std::string	perFrameVars[] =
	{
		"projection",
		"view",
		"time",
		"pointLightNbr",
		"spotLightNbr"
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

	std::string	perLightVars[] =
	{
		"lightVP"
	};

	getInstance<Renderer>()->addUniform("MaterialBasic")
		->init(s, "MaterialBasic", materialBasic);
	getInstance<Renderer>()->addUniform("PerFrame")
		->init(s, "PerFrame", perFrameVars);
	getInstance<Renderer>()->addUniform("PerModel")
		->init(s, "PerModel", perModelVars);
	getInstance<Renderer>()->addUniform("PerLight")
		->init(shadowDepth, "PerLight", perLightVars);

	getInstance<Renderer>()->addShader("2DText",
		"../../Shaders/2DText.vp",
		"../../Shaders/2DText.fp");

	getInstance<Renderer>()->addShader("SpriteBasic",
		"../../Shaders/SpriteBasic.vp",
		"../../Shaders/SpriteBasic.fp");

	// _engine.getInstance<Renderer>()->addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/basic.gp");
	getInstance<Renderer>()->addShader("basicLight", "../../Shaders/light.vp", "../../Shaders/light.fp");
	// _engine.getInstance<Renderer>()->addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
	// _engine.getInstance<Renderer>()->addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");
	getInstance<Renderer>()->addShader("depthOnly", "../../Shaders/depthOnly.vp", "../../Shaders/depthOnly.fp");
	getInstance<Renderer>()->bindShaderToUniform("ShadowDepth", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("ShadowDepth", "PerLight", "PerLight");

	getInstance<Renderer>()->bindShaderToUniform("depthOnly", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("depthOnly", "PerModel", "PerModel");

	// _engine.getInstance<Renderer>()->bindShaderToUniform("basic", "PerFrame", "PerFrame");
	// _engine.getInstance<Renderer>()->bindShaderToUniform("basic", "PerModel", "PerModel");

	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__cube.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__ball.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__sponza.cpd"));
	//	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__SketchTest.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Conference.cpd"));

	getInstance<FontManager>()->loadFont(File("../../Assets/Serialized/myFont.cpdFont"));

	getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/GreyMan.CPDAnimation"));



	// EXAMPLE: HOW TO CREATE A MEDIA FILE DYNAMICALY
	//auto defaultBallMesh = getInstance<AssetsManager>()->get<ObjFile>("obj__ball");
	//auto planetMesh = getInstance<AssetsManager>()->create<ObjFile>("my_planet", defaultBallMesh);
	//planetMesh->material = getInstance<AssetsManager>()->create<MaterialFile>("my_planet_material", defaultBallMesh->material);
	//auto testsss = planetMesh->material->materials[0];
	//planetMesh->material->materials[0].ambientTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthTexture");
	//planetMesh->material->materials[0].diffuseTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthNightTexture");
	//planetMesh->material->materials[0].specularTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthClouds");
	//planetMesh->material->materials[0].normalTex = getInstance<AssetsManager>()->get<TextureFile>("texture__EarthTextureBump");

	// EXAMPLE: HOW TO SAVE TO FILE A MEDIA FILE CREATED DYNAMICALY
	//getInstance<AssetsManager>()->saveToFile("my_planet_material", "./Assets/Serialized/");
	//getInstance<AssetsManager>()->saveToFile("my_planet", "./Assets/Serialized/");

	// EXAMPLE LOAD FROM SAVE
	//getInstance<AssetsManager>()->loadFromFile(File("../../Assets/Serialized/my_planet.cpd"));


//	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__cube.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__ball.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	//getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__sponza.cpd"));
	//	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__SketchTest.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
//	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__Museum.cpd"));


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
		e->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "idle"));
		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0, 300, 0)));
		auto e2 = createEntity();
		e2->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "walk"));
		e2->setLocalTransform(glm::translate(e2->getLocalTransform(), glm::vec3(1700, 0, 0)));
		auto e3 = createEntity();
		e3->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "crouch_idle"));
		e3->setLocalTransform(glm::translate(e3->getLocalTransform(), glm::vec3(1700, 400, 0)));
		auto e4 = createEntity();
		e4->addComponent<Component::Sprite>(getInstance<SpriteManager>()->getAnimation("GreyMan", "kitten"));
		e4->setLocalTransform(glm::translate(e4->getLocalTransform(), glm::vec3(1700, 800, 0)));
		e4->setLocalTransform(glm::scale(e4->getLocalTransform(), glm::vec3(0.1)));
	}


	// CREATE SPONZA CHURCH
	{
		auto e = createEntity();
		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0)));
		e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(70)));

		// e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(70, 1, 70));
		auto rigidBody = e->addComponent<Component::RigidBody>(0);
		rigidBody->setMass(0);
		rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sponza");
		// rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sketch-test");
		// rigidBody->setCollisionShape(Component::RigidBody::BOX);
		// rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_museum");

		rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
		rigidBody->getShape().setMargin(0.001f);
		rigidBody->getBody().setFriction(1.0f);
		rigidBody->getBody().setRestitution(0.9f);

		// auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__sketch-test"));
		// auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__cube"));
		auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__sponza"));
		// auto mesh = e->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__museum"));

		mesh->setShader("MaterialBasic");
	}

	Entity character;
	std::shared_ptr<Component::CameraComponent> cameraComponent1;
	std::shared_ptr<Component::CameraComponent> cameraComponent2;

	{
		auto e = createEntity();
		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(0, 100, 0)));
		auto fpc = e->addComponent<Component::FPController>();
		character = e;
		cameraComponent1 = character->addComponent<Component::CameraComponent>();
		character->addComponent<Component::FirstPersonView>();
		globalCamera = e;
	}


	{
		auto e = createMonkey(glm::vec3(19, 0.9, -0.59), glm::vec3(1.5), "texture__SunTexture", 1.0f);
		auto rigidbody = e->getComponent<Component::RigidBody>();
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;

	}

	// --
	// Setting camera with skybox
	// --

	std::string	vars[] =
	{
		"projection",
		"view"
	};

	auto sky = getInstance<Renderer>()->addShader("cubemapShader", "../../Shaders/cubemap.vp", "../../Shaders/cubemap.fp");

	getInstance<Renderer>()->getShader("cubemapShader")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();

	getInstance<Renderer>()->addUniform("cameraUniform")
		->init(sky, "cameraUniform", vars);

	getInstance<Renderer>()->bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");

	auto screenSize = getInstance<IRenderContext>()->getScreenSize();
	cameraComponent1->attachSkybox("skybox__space", "cubemapShader");
	cameraComponent1->viewport = glm::uvec4(0, 0, screenSize.x / 2, screenSize.y);
	cameraComponent1->fboSize = glm::uvec2(screenSize.x / 2, screenSize.y);
	cameraComponent1->sampleNbr = 2;

	// init frame buffer and send texture id to other scene
	cameraComponent1->initFrameBuffer();
	OpenGLTools::Framebuffer &current = cameraComponent1->frameBuffer.isMultisampled() ? cameraComponent1->downSampling : cameraComponent1->frameBuffer;
	auto psm = getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>();
	_globalPubSub = std::make_unique<PubSub>(psm);
	_globalPubSub->broadCast(PubSubKey("fboSponzaId"), current.getTextureAttachment(GL_COLOR_ATTACHMENT0));

	_globalPubSub->globalSub(PubSubKey("sponzaPause"), [&](){
		deactivateSystem<FPControllerSystem>(); // UPDATE FIRST PERSON CONTROLLER
	});

	_globalPubSub->globalSub(PubSubKey("sponzaPlay"), [&](){
		activateSystem<FPControllerSystem>(); // UPDATE FIRST PERSON CONTROLLER
	});


	return (true);
}

bool SponzaScene::userUpdate(double time)
{
	static std::queue<Entity> stack;
	float ftime = static_cast<float>(time);

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
				if (e->isTagged(BULLET_TAG))
					destroy(e);
			}
		}
	}
	static float delay = 0.0f;
	if (getInstance<Input>()->getInput(SDL_BUTTON_LEFT) && delay <= 0.0f)
	{
		glm::vec3 from, to;
		getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
		auto e = createSphere(from + to * 1.5f, glm::vec3(0.2f), "on s'en bas la race", 10.0f);
		auto rigidbody = e->getComponent<Component::RigidBody>();

		rigidbody->getBody().applyCentralImpulse(convertGLMVectorToBullet(to * 80.0f));
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		rigidbody->getBody().getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;

		auto light = e->addComponent<Component::PointLight>();
		light->lightData.colorRange = glm::vec4(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, 5.0f);
		light->lightData.positionPower.w = 3.0f;
		rigidbody->getBody().setFriction(1.0f);
		rigidbody->getBody().setRestitution(0.9f);

		auto &body = rigidbody->getBody();
		body.applyCentralImpulse(convertGLMVectorToBullet(to * 10.0f));
		body.getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
		body.getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;
		body.setFriction(1.0f);
		body.setRestitution(0.9f);

		e->addTag(BULLET_TAG);
		if (stack.size() > 50)
		{
			destroy(stack.front());
			stack.pop();
		}
		stack.push(e);
		delay = 0.1f;
	}
	if (getInstance<Input>()->getInput(SDL_BUTTON_MIDDLE) && delay <= 0.0f)
	{
		auto e = createEntity();

		auto l = e->addComponent<Component::SpotLight>();
		auto cam = globalCamera->getComponent<Component::CameraComponent>();
		l->projection = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
		l->lightData.colorRange = glm::vec4(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, 100.0f);
		l->lightData.positionPower.w = 50.0f;
		l->lightData.shadowId = 1;
		e->setLocalTransform(glm::inverse(cam->lookAtTransform));
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
	if (getInstance<Input>()->getInput(SDLK_h))
		getSystem<PostFxSystem>()->useHDR(true);
	if (getInstance<Input>()->getInput(SDLK_j))
		getSystem<PostFxSystem>()->useHDR(false);
	if (getInstance<Input>()->getInput(SDLK_f))
		getSystem<PostFxSystem>()->useBloom(true);
	if (getInstance<Input>()->getInput(SDLK_g))
		getSystem<PostFxSystem>()->useBloom(false);
	static float	sigma = 5.0f;
	static float	glare = 1.0f;
	if (getInstance<Input>()->getInput(SDLK_UP))
	{
		sigma += 0.5f;
		getSystem<PostFxSystem>()->setBloomSigma(sigma);
	}
	if (getInstance<Input>()->getInput(SDLK_DOWN))
	{
		sigma -= 0.5f;
		getSystem<PostFxSystem>()->setBloomSigma(sigma);
	}
	if (getInstance<Input>()->getInput(SDLK_LEFT))
	{
		glare -= 0.2f;
		getSystem<PostFxSystem>()->setBloomGlare(glare);
	}
	if (getInstance<Input>()->getInput(SDLK_RIGHT))
	{
		glare += 0.2f;
		getSystem<PostFxSystem>()->setBloomGlare(glare);
	}
	static auto timeCounter = 0.0f;
	static auto frameCounter = 0;
	static auto lastFrame = 0;
	timeCounter += ftime;
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
