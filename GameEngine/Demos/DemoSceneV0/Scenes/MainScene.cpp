#include "MainScene.hh"

#include <glm/gtx/matrix_operation.hpp>

// components
#include <Components/FPController.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/FirstPersonView.hpp>

//systems
#include <SharedSystems/FPSSystem.hh>
#include <Systems/FPControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/MeshRenderSystem.h>
#include <Systems/BulletDynamicSystem.hpp>

// SDL
#include <Context/SdlContext.hh>

MainScene::MainScene(std::weak_ptr<Engine> engine)
: AScene(engine)
{}

MainScene::~MainScene(void)
{}


bool 			MainScene::userStart()
{
	// add systems
	addSystem<MeshRendererSystem>(0);
	addSystem<FPControllerSystem>(10);
	addSystem<FirstPersonViewSystem>(20);
	addSystem<CameraSystem>(30);
	addSystem<BulletDynamicSystem>(35);
	addSystem<FPSSystem>(40);

	//load shaders
	if (!loadShaders())
		return false;
	if (!loadAssets())
		return false;

	// create heros
	{
		_heros = createEntity();

		_heros->setLocalTransform(glm::translate(_heros->getLocalTransform(), glm::vec3(-8, 2, 0)));

		auto fpc = _heros->addComponent<Component::FPController>();
		fpc->getShape().setLocalScaling(btVector3(0.3f, 0.3f, 0.3f));
		auto camera = _heros->addComponent<Component::CameraComponent>();
		camera->attachSkybox("skybox__space", "cubemapShader");
		auto fpv = _heros->addComponent<Component::FirstPersonView>();
	}

	// create Entrance room
	{
		_entrance = std::make_unique<Entrance>(
			std::dynamic_pointer_cast<AScene>(shared_from_this())
			);
		if (!_entrance->init())
			return false;
		_entrance->enable();
	}

	return true;
}


bool 			MainScene::userUpdate(double time)
{
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
	{
		return false;
	}
	return true;
}

bool MainScene::loadShaders()
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

	std::string		materialBasic[] =
	{
		"ambient",
		"diffuse",
		"specular",
		"transmittance",
		"emission",
		"shininess"
	};

	std::string		vars[] =
	{
		"projection",
		"view"
	};

	auto s = getInstance<Renderer>()->addShader("MaterialBasic",
		"../../Shaders/MaterialBasic.vp",
		"../../Shaders/MaterialBasic.fp");

	getInstance<Renderer>()->addUniform("MaterialBasic")
		->init(s, "MaterialBasic", materialBasic);
	getInstance<Renderer>()->addUniform("PerFrame")
		->init(s, "PerFrame", perFrameVars);
	getInstance<Renderer>()->addUniform("PerModel")
		->init(s, "PerModel", perModelVars);

	auto sky = getInstance<Renderer>()->addShader("cubemapShader", "../../Shaders/cubemap.vp", "../../Shaders/cubemap.fp");

	getInstance<Renderer>()->getShader("cubemapShader");

	getInstance<Renderer>()->addUniform("cameraUniform")
		->init(sky, "cameraUniform", vars);

	getInstance<Renderer>()->bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");

	getInstance<Renderer>()->getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();

	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");
	return true;
}

bool MainScene::loadAssets()
{
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	return true;
}