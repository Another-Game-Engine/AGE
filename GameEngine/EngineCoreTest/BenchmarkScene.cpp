#include <Scenes/BenchmarkScene.hpp>
#include <Systems/LifetimeSystem.hpp>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DownSampleSystem.hh>
#include <Systems/PostFxSystem.hh>
#include <Systems/LightRenderingSystem.hh>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/BlitFinalRender.hh>
#include <Context/IRenderContext.hh>
#include <Components/MeshRenderer.hh>
#include <CONFIGS.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BenchmarkScene::BenchmarkScene(std::weak_ptr<Engine> &&engine)
: AScene(std::move(engine))
{
}

BenchmarkScene::~BenchmarkScene()
{
}

bool BenchmarkScene::userStart()
{
#if PHYSIC_SIMULATION
	addSystem<BulletDynamicSystem>(0);
	addSystem<CollisionAdder>(1);
	addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

#if RENDERING_ACTIVATED
	addSystem<FirstPersonViewSystem>(2);
	addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	addSystem<LightRenderingSystem>(80); // Render with the lights
	addSystem<DownSampleSystem>(100); // DOWNSAMPLE FBO
	addSystem<PostFxSystem>(110); // POST FXs
	addSystem<BlitFinalRender>(120); // BLIT ON FBO 0

	getSystem<PostFxSystem>()->setHDRIdealIllumination(0.3f);
	getSystem<PostFxSystem>()->setHDRAdaptationSpeed(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxLightDiminution(0.1f);
	getSystem<PostFxSystem>()->setHDRMaxDarkImprovement(1.2f);
	getSystem<PostFxSystem>()->useHDR(false);
	getSystem<PostFxSystem>()->useBloom(false);

#endif

#if LIFETIME_ACTIVATED
	addSystem<LifetimeSystem>(2);
#endif //!LIFETIME_ACTIVATED

	srand(42);

#if LOG_FRAMERATE
	_logFile.open("LogFile.log", std::ios::app);
	_logFile << "\n\nNew test in ";
#if _DEBUG
	_logFile << "DEBUG . With :";
#else
	_logFile << "RELEASE . With :";
#endif

#if LIFETIME_ACTIVATED
	_logFile << " Lifetime, ";
#endif

#if COMPLEX_MESH
	_logFile << " Complex mesh, ";
#elif RENDERING_ACTIVATED
	_logFile << " Rendering, ";
#endif

#if PHYSIC_SIMULATION
	_logFile << " Physics, ";
#endif
	_logFile << "\n";


#endif

#if RENDERING_ACTIVATED

	auto camera = createEntity();
	auto cam = camera->addComponent<Component::CameraComponent>();
	camera->addComponent<Component::FirstPersonView>();

	auto screenSize = getInstance<IRenderContext>()->getScreenSize();
	cam->fboSize = screenSize;
	cam->viewport = glm::uvec4(0, 0, cam->fboSize.x, cam->fboSize.y);
	cam->attachSkybox("skybox__space", "cubemapShader");
	cam->sampleNbr = 0;

	camera->setLocalTransform(glm::translate(glm::mat4(1), glm::vec3(0, 0, -40)));


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

#endif
	return true;
}

bool BenchmarkScene::userUpdate(double time)
{
	++_frameCounter;
	++_chunkFrame;
	_timeCounter += time;
	_chunkCounter += time;

	if (_chunkCounter >= _maxChunk)
	{
		for (auto i = 0; i < 14; ++i)
		{
			auto e = createEntity();
#if LIFETIME_ACTIVATED
			e->addComponent<Component::Lifetime>(LIFETIME_RATE);
#endif

#if PHYSIC_SIMULATION
			auto rigidBody = e->addComponent<Component::RigidBody>(1.0f);
			rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(1.0f);
#endif
#if RENDERING_ACTIVATED



#if !COMPLEX_MESH
			auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
			mesh->setShader("MaterialBasic");
#else
			auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
			mesh->setShader("MaterialBasic");
#endif

#endif
			e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3((rand() % 20) - 10, (rand() % 20) - 5, (rand() % 20) - 10)));
		}
#if LOG_FRAMERATE
		_logFile << _chunkFrame << ", ";
		_chunkCounter = 0.0;
		_chunkFrame = 0;
	}
	if (_timeCounter >= _maxTime)
	{
		_logFile << std::endl << "Total frames : " << _frameCounter << " -- Entity created : " << this->getNumberOfEntities() << std::endl << "----------------" << std::endl;
		_logFile.close();
		return false;
	}
#endif
	return true;
}