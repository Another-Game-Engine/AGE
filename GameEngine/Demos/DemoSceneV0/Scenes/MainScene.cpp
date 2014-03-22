#include "MainScene.hh"

#include <glm/gtx/matrix_operation.hpp>

// components
#include <Components/FPController.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/FirstPersonView.hpp>

//systems
#include <Systems/FPSSystem.hh>
#include <Systems/FPControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/LightRenderingSystem.hh>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/SpriteSystem.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/DownSampleSystem.hh>
#include <Systems/PostFxSystem.hh>
#include <Systems/BlitFinalRender.hh>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/HotZoneSystem.hpp>
#include <MyTags.hpp>

// SDL
#include <Context/SdlContext.hh>

MainScene::MainScene(std::weak_ptr<Engine> engine)
: AScene(engine)
{}

MainScene::~MainScene(void)
{}


bool 			MainScene::userStart()
{
	setInstance<BulletDynamicManager, BulletCollisionManager>()->init();

	std::dynamic_pointer_cast<BulletDynamicManager>(getInstance<BulletCollisionManager>())->getWorld()->setGravity(btVector3(0, -10, 0));

	//load shaders
	if (!loadShaders())
		return false;
	if (!loadAssets())
		return false;

	// add systems
	addSystem<TransformationRegisterSystem>(0)->setFile(File("../../EntityTransformationSave.json"));
//	addSystem<EntityPlacingSystem>(0);
	addSystem<FPControllerSystem>(10);
	addSystem<FirstPersonViewSystem>(20);
	addSystem<CameraSystem>(30);
	addSystem<BulletDynamicSystem>(35);
	addSystem<CollisionAdder>(36);
	addSystem<HotZoneSystem>(37);
	addSystem<FPSSystem>(40);

	addSystem<LightRenderingSystem>(80); // Render with the lights
	addSystem<SpriteSystem>(90); // DRAW SPRITES
	addSystem<DownSampleSystem>(100); // DOWNSAMPLE FBO
	addSystem<PostFxSystem>(110); // POST FXs
	addSystem<BlitFinalRender>(120); // BLIT ON FBO 0
	addSystem<CollisionCleaner>(180);

	getSystem<PostFxSystem>()->setHDRIdealIllumination(0.3f);
	getSystem<PostFxSystem>()->setHDRAdaptationSpeed(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxLightDiminution(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxDarkImprovement(1.2f);
	getSystem<PostFxSystem>()->useHDR(false);
	getSystem<PostFxSystem>()->useBloom(false);

	// create heros
	{
		_heros = createEntity();

		_heros->setLocalTransform(glm::translate(_heros->getLocalTransform(), glm::vec3(-49, 1, 0)));
		auto camera = _heros->addComponent<Component::CameraComponent>();
		auto screenSize = getInstance<IRenderContext>()->getScreenSize();
		camera->viewport = glm::uvec4(0, 0, screenSize.x, screenSize.y);
		camera->attachSkybox("skybox__space", "cubemapShader");
		auto fpv = _heros->addComponent<Component::FirstPersonView>();
		auto l = _heros->addComponent<Component::PointLight>();
		l->lightData.colorRange = glm::vec4(0.8f,1.0f,1.0f, 20.0f);
		l->lightData.positionPower.w = 2.0f;
//		_heros->addComponent<Component::TransformationRegister>("character-controller-museum");
		auto fpc = _heros->addComponent<Component::FPController>();
		fpc->getShape().setLocalScaling(btVector3(0.3f, 0.3f, 0.3f));
		fpc->backwardRunSpeed = 0.001f;
		fpc->forwardRunSpeed = 0.001f;
		fpc->sideRunSpeed = 0.001f;
		_heros->addTag(MyTags::HEROS_TAG);

		auto herosLight = createEntity();
		l = herosLight->addComponent<Component::PointLight>();
		l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f);
		l->lightData.positionPower.w = 10.0f;
		herosLight->addComponent<Component::EntityPlacable>("HEROS-LIGHT");
		herosLight->addComponent<Component::TransformationRegister>("HEROS-LIGHT");

	}

	// create Entrance room
	{
		_entrance = std::make_shared<Entrance>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_entrance->init())
			return false;
		_entrance->enable();
	}

	// create Engine room
	{
		_engineRoom = std::make_shared<EngineRoom>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_engineRoom->init())
			return false;
	}

	// create Projection room
	{
		_projectionRoom = std::make_shared<ProjectionRoom>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_projectionRoom->init())
			return false;
	}

	// create Physics room
	{
		_physicsRoom = std::make_shared<PhysicsRoom>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_physicsRoom->init())
			return false;
	}

	// create Last room
	{
		_lastRoom = std::make_shared<LastRoom>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_lastRoom->init())
			return false;
	}

	// create Last room
	{
		_circleRoom = std::make_shared<CircleRoom>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_circleRoom->init())
			return false;
	}

	return true;
}


bool 			MainScene::userUpdate(double time)
{
	float ftime = (float)(time);
	static float delay = 0.0f;
	if (getInstance<Input>()->getInput(SDLK_r) && delay <= 0.0f)
	{
		if (_entrance->isEnable())
			_entrance->disable();
		else
			_entrance->enable();

//		glm::vec3 from, to;
//		getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
//		auto e = createEntity();
//		e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(from + to * 1.5f)));
//		e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.3f)));
////		e->setLocalTransform(glm::lookAt(from, to, glm::vec3(0,1,0)));
//		auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__cube"));
//		mesh->setShader("MaterialBasic");
//		auto rigidBody = e->addComponent<Component::RigidBody>(0.05f);
//		rigidBody->setCollisionShape(Component::RigidBody::BOX);
//		//auto l = e->addComponent<Component::PointLight>();
//		//l->lightData.colorRange = glm::vec4(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, 5.0f);
//		//l->lightData.positionPower.w = 3.0f;
		delay = 0.1f;
	}
	if (delay >= 0.0f)
		delay -= ftime;
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
	{
		return false;
	}
	if (getInstance<Input>()->getInput(SDLK_m))
	{
		getInstance<SceneManager>()->enableScene("SpaceGame", 0);
		getInstance<SceneManager>()->disableScene("MainScene");
	}
	return true;
}

bool MainScene::loadShaders()
{
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

	auto s = getInstance<Renderer>()->addShader("MaterialBasic",
		"../../Shaders/MaterialBasic.vp",
		"../../Shaders/MaterialBasic.fp");

	auto shadowDepth = getInstance<Renderer>()->addShader("ShadowDepth" , "../../Shaders/ShadowMapping.vp", "../../Shaders/ShadowMapping.fp");

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


	getInstance<Renderer>()->addShader("basicLight", "../../Shaders/light.vp", "../../Shaders/light.fp");
	getInstance<Renderer>()->addShader("depthOnly", "../../Shaders/depthOnly.vp", "../../Shaders/depthOnly.fp");
	getInstance<Renderer>()->bindShaderToUniform("ShadowDepth", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("ShadowDepth", "PerLight", "PerLight");

	getInstance<Renderer>()->bindShaderToUniform("depthOnly", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("depthOnly", "PerModel", "PerModel");

	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

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

	return true;
}

bool MainScene::loadAssets()
{
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__cube.cpd"));
	return true;
}