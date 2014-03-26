#include <Scenes/SpaceGame.hh>

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
#include <Systems/AsteroidSystem.hh>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>

#include <glm/glm.hpp>

#include "MyTags.hpp"

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
	setInstance<FontManager>()->init();

	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__torus.cpd"));

	addSystem<CollisionAdder>(10);

	addSystem<CameraSystem>(30);
	addSystem<BulletDynamicSystem>(35);
	addSystem<SpaceshipControllerSystem>(40);
	addSystem<AsteroidSystem>(50);

	addSystem<CollisionCleaner>(60);

	addSystem<LightRenderingSystem>(80); // Render with the lights
	addSystem<SpriteSystem>(90); // DRAW SPRITES
	addSystem<DownSampleSystem>(100); // DOWNSAMPLE FBO
	addSystem<PostFxSystem>(110); // POST FXs
//	addSystem<BlitFinalRender>(120);

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
	auto rigidBody = spaceShip->addComponent<Component::RigidBody>(1.0f);
	rigidBody->setCollisionShape(Component::RigidBody::BOX);
	rigidBody->getBody().setActivationState(DISABLE_DEACTIVATION);
	rigidBody->getBody().setDamping(0.5f, 1.0f);

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
	controller->angularSpeed = 80.0f;
	controller->acceleration = 20.0f;
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
	cameraComponent->sampleNbr = 1;
	// init frame buffer and send texture id to other scene
	cameraComponent->initFrameBuffer();
	OpenGLTools::Framebuffer &current = cameraComponent->frameBuffer.isMultisampled() ? cameraComponent->downSampling : cameraComponent->frameBuffer;
	auto psm = getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>();
	_globalPubSub = std::make_unique<PubSub>(psm);
	_globalPubSub->broadCast(PubSubKey("fboAsteroidId"), current.getTextureAttachment(GL_COLOR_ATTACHMENT0));

	_globalPubSub->globalSub(PubSubKey("asteroidPause"), [&](){
		deactivateSystem<SpaceshipControllerSystem>(); // UPDATE FIRST PERSON CONTROLLER
	});

	_globalPubSub->globalSub(PubSubKey("asteroidPlay"), [&](){
		activateSystem<SpaceshipControllerSystem>(); // UPDATE FIRST PERSON CONTROLLER
	});


	for (int i = 0; i < 30; ++i)
	{
		Entity e = createEntity();

		glm::vec3 impulse = glm::vec3(rand() % 10000, rand() % 10000, rand() % 10000);

		impulse = glm::normalize(impulse);
		impulse *= 1000.0f;

		glm::vec3 position = glm::vec3(cos(2 * 3.141592 / 100 * i), 0, sin(2 * 3.141592 / 100 * i));

		position *= 50.0f;

		e->setLocalTransform(glm::translate(glm::mat4(1), position));
		e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(10.0f)));

		auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
		mesh->setShader("MaterialBasic");
		auto body = e->addComponent<Component::RigidBody>(50);
		body->setCollisionShape(Component::RigidBody::SPHERE);
		body->getBody().setRestitution(1.0f);
		body->getBody().setActivationState(DISABLE_DEACTIVATION);
		body->getBody().setDamping(0.0f, 0.0f);
		body->getBody().applyCentralImpulse(convertGLMVectorToBullet(impulse));
		e->addTag(MyTags::ASTEROID_TAG);
	}


	return (true);
}

bool 			SpaceGame::userUpdate(double time)
{
	return (true);
}
