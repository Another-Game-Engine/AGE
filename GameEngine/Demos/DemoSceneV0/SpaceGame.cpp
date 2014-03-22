#include <SpaceGame.hh>

// components
#include <Components/FPController.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/SpaceshipController.hpp>

//systems
#include <Systems/SpaceshipControllerSystem.hh>
#include <Systems/CameraSystem.hpp>
#include <Systems/LightRenderingSystem.hh>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/SpriteSystem.hh>
#include <Systems/DownSampleSystem.hh>
#include <Systems/PostFxSystem.hh>
#include <Systems/BlitFinalRender.hh>
#include <Systems/FPSSystem.hh>

#include <glm/glm.hpp>

SpaceGame::SpaceGame(std::weak_ptr<Engine> engine) :
							AScene(engine)
{
}

SpaceGame::~SpaceGame()
{
}

bool 			SpaceGame::userStart()
{
	setInstance<BulletDynamicManager, BulletCollisionManager>()->init();

	std::dynamic_pointer_cast<BulletDynamicManager>(getInstance<BulletCollisionManager>())->getWorld()->setGravity(btVector3(0, 0, 0));

	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__torus.cpd"));

	addSystem<CameraSystem>(30);
	addSystem<BulletDynamicSystem>(35);
	addSystem<SpaceshipControllerSystem>(40);

	addSystem<LightRenderingSystem>(80); // Render with the lights
	addSystem<SpriteSystem>(90); // DRAW SPRITES
	addSystem<DownSampleSystem>(100); // DOWNSAMPLE FBO
	addSystem<PostFxSystem>(110); // POST FXs
	addSystem<BlitFinalRender>(120); // BLIT ON FBO 0
	addSystem<FPSSystem>(130);
	getSystem<PostFxSystem>()->setHDRIdealIllumination(0.3f);
	getSystem<PostFxSystem>()->setHDRAdaptationSpeed(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxLightDiminution(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxDarkImprovement(1.2f);
	getSystem<PostFxSystem>()->useHDR(false);
	getSystem<PostFxSystem>()->useBloom(false);

	auto light = createEntity();
	auto lightComponent = light->addComponent<Component::PointLight>();
	lightComponent->lightData.colorRange = glm::vec4(1, 1, 1, 50);
	lightComponent->lightData.positionPower.w = 2.0f;
	lightComponent->lightData.hasShadow = -1;
	light->setLocalTransform(glm::translate(glm::mat4(1), glm::vec3(0, 2, 0)));

	light = createEntity();
	lightComponent = light->addComponent<Component::PointLight>();
	lightComponent->lightData.colorRange = glm::vec4(1, 0.5, 0.5, 3);
	lightComponent->lightData.positionPower.w = 0;
	lightComponent->lightData.hasShadow = -1;
	light->setLocalTransform(glm::translate(glm::mat4(1), glm::vec3(0, 0, -2)));

	auto spaceShip = createEntity();

	spaceShip->addChild(light);

	spaceShip->setLocalTransform(glm::translate(spaceShip->getLocalTransform(), glm::vec3(50, 0, 0)));
	spaceShip->setLocalTransform(glm::scale(spaceShip->getLocalTransform(), glm::vec3(1.0f)));
	auto meshObj = getInstance<AssetsManager>()->get<ObjFile>("obj__galileo");
	if (!meshObj)
		return false;
	auto meshComponent = spaceShip->addComponent<Component::MeshRenderer>(meshObj);
	meshComponent->setShader("MaterialBasic");
	auto rigidBody = spaceShip->addComponent<Component::RigidBody>(0.0f);
	rigidBody->setMass(1.0f);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	rigidBody->getBody().setActivationState(DISABLE_DEACTIVATION);
	rigidBody->getBody().setDamping(0.3f, 0.3f);

	auto torus = createEntity();
	torus->setLocalTransform(glm::scale(torus->getLocalTransform(), glm::vec3(200.0f)));
	meshObj = getInstance<AssetsManager>()->get<ObjFile>("obj__torus");
	if (!meshObj)
		return false;
	meshComponent = torus->addComponent<Component::MeshRenderer>(meshObj);
	meshComponent->setShader("MaterialBasic");
	rigidBody = torus->addComponent<Component::RigidBody>(0.0f);
	rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_torus");

	auto e = createEntity();
	auto cameraComponent = e->addComponent<Component::CameraComponent>();
	auto controller = e->addComponent<Component::SpaceshipController>(spaceShip, 5.0f);

	controller->dist = 5.0f;
	controller->angularSpeed = 3.0f;
	controller->acceleration = 0.2f;
	controller->light = lightComponent;
	controller->camSpeed = 5.0f;

	light = createEntity();
	lightComponent = light->addComponent<Component::PointLight>();
	lightComponent->lightData.colorRange = glm::vec4(0.5, 0.5, 1, 200.0f);
	lightComponent->lightData.positionPower.w = 5.0f;
	lightComponent->lightData.hasShadow = -1;

	spaceShip->addChild(light);

	auto sky = getInstance<Renderer>()->getShader("cubemapShader");

	getInstance<Renderer>()->getShader("cubemapShader");

	auto screenSize = getInstance<IRenderContext>()->getScreenSize();
	cameraComponent->attachSkybox("skybox__space", "cubemapShader");
	cameraComponent->viewport = glm::uvec4(0, 0, screenSize.x, screenSize.y);
	cameraComponent->projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
	cameraComponent->lookAtTransform = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0), glm::vec3(0, 1, 0));
	cameraComponent->fboSize = getInstance<IRenderContext>()->getScreenSize();
	cameraComponent->viewport = glm::vec4(0, 0, cameraComponent->fboSize.x, cameraComponent->fboSize.y);
	cameraComponent->sampleNbr = 1;

	return (true);
}

bool 			SpaceGame::userUpdate(double time)
{
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
	{
		return false;
	}
	if (getInstance<Input>()->getInput(SDLK_l))
	{
		getInstance<SceneManager>()->enableScene("MainScene", 0);
		getInstance<SceneManager>()->disableScene("SpaceGame");
	}
	return (true);
}
