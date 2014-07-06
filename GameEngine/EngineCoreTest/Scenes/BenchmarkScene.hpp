#pragma once

#include <Core/AScene.hh>

#include <Systems/LifetimeSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>

#include <Systems/CameraSystem.hh>
#include <Systems/DownSampleSystem.hh>
#include <Systems/PostFxSystem.hh>
#include <Systems/LightRenderingSystem.hh>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/BlitFinalRender.hh>
#include <Systems/SimpleMeshRenderer.hpp>

#include <Core/Octree.hpp>

#include <Context/IRenderContext.hh>

#include <CONFIGS.hpp>

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<Engine> &&engine)
		: AScene(std::move(engine))
	{}

	virtual ~BenchmarkScene(void)
	{}

	virtual bool 			userStart()
	{

		setInstance<AGE::Octree>();

#ifdef PHYSIC_SIMULATION
		addSystem<BulletDynamicSystem>(0);
//		addSystem<CollisionAdder>(1);
//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

#ifdef RENDERING_ACTIVATED

	addSystem<FirstPersonViewSystem>(2);
	auto &camerasystem = addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	auto &m = *getInstance<gl::ShadingManager>();
	auto &g = *getInstance<gl::GeometryManager>();
#if NEW_SHADER
	camerasystem->setManager(m, g);
#endif

#ifdef SIMPLE_RENDERING
		addSystem<SimpleMeshRenderer>(80);
#else
#if !NEW_SHADER
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
#endif
#endif

#ifdef LIFETIME_ACTIVATED
		addSystem<LifetimeSystem>(2);
#endif //!LIFETIME_ACTIVATED

		srand(42);

#ifdef LOG_FRAMERATE
		_logFile.open("LogFile.log", std::ios::app);
		_logFile << "\n\nNew test in ";
#ifdef _DEBUG
		_logFile << "DEBUG . With :";
#else
		_logFile << "RELEASE . With :";
#endif
#ifdef LIFETIME_ACTIVATED
		_logFile << " Lifetime, ";
#endif
#ifdef COMPLEX_MESH
			_logFile << " Complex mesh, ";
#elif defined RENDERING_ACTIVATED
			_logFile << " Rendering, ";
#endif
#ifdef PHYSIC_SIMULATION
		_logFile << " Physics, ";
#endif
		_logFile << "\n";


#endif

#ifdef RENDERING_ACTIVATED

		auto camera = createEntity();
		auto cam = addComponent<Component::CameraComponent>(camera);
		addComponent<Component::FirstPersonView>(camera);

		auto screenSize = getInstance<IRenderContext>()->getScreenSize();
		cam->fboSize = screenSize;
		cam->viewport = glm::uvec4(0, 0, cam->fboSize.x, cam->fboSize.y);
		cam->attachSkybox(getInstance<AssetsManager>()->get<CubeMapFile>("skybox__space"), "cubemapShader");
		cam->sampleNbr = 0;

		auto camLink = getLink(camera);
		camLink->setPosition(glm::vec3(0, 0, -40));

	auto light = createEntity();
	auto lightLink = getLink(light);
	auto lightComponent = addComponent<Component::PointLight>(light);
	lightComponent->lightData.colorRange = glm::vec4(1, 1, 1, 30);
	lightComponent->lightData.positionPower.w = 0.5f;
	lightComponent->lightData.hasShadow = -1;
	lightLink->setPosition(glm::vec3(0, 3, 0));

	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
	auto plane = createEntity();
	auto link = getLink(plane);
	link->setPosition(glm::vec3(0, -10, 0));
	link->setScale(glm::vec3(100, 1, 100));
	auto mesh = addComponent<Component::MeshRenderer>(plane, getInstance<AssetsManager>()->get<ObjFile>("obj__cube"));
	mesh->setShader("MaterialBasic");
	auto rigidBody = addComponent<Component::RigidBody>(plane, 0.0f);
	rigidBody->setCollisionShape(weakOnThis, plane, Component::RigidBody::BOX);
//	rigidBody->setTransformation(getTransform(plane));
	rigidBody->getBody().setFriction(0.8f);
	//rigidBody->getBody().setRestitution(1.0f);

#endif


		return true;
	}

	virtual bool 			userUpdate(double time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;

		if (_chunkCounter >= _maxChunk)
		{
			std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
			for (auto i = 0; i < 7; ++i)
			{
				auto e = createEntity();

#ifdef LIFETIME_ACTIVATED
				addComponent<Component::Lifetime>(e, 10.5f);
#endif

#ifdef PHYSIC_SIMULATION
				auto rigidBody = addComponent<Component::RigidBody>(e, 1.0f);
				if (i % 4 == 0)
					rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::SPHERE);
				else
					rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::BOX);
				rigidBody->getBody().setFriction(0.5f);
				rigidBody->getBody().setRestitution(0.5f);
#endif
				auto link = getLink(e);
				link->setPosition(glm::vec3((rand() % 20) - 10, (rand() % 20) - 5, (rand() % 20) - 10));
				link->setScale(glm::vec3(3.0f));

#ifdef RENDERING_ACTIVATED

				if (i == 0)
				{
					auto lightComponent = addComponent<Component::PointLight>(e);
					lightComponent->lightData.colorRange = glm::vec4((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 13);
					lightComponent->lightData.positionPower.w = 1;
					lightComponent->lightData.hasShadow = 1;
				}


#ifdef PHYSIC_SIMULATION
				rigidBody->setTransformation(link->getTransform());
#endif

#ifndef COMPLEX_MESH
				Component::MeshRenderer *mesh;
				if (i % 4 == 0)
					mesh = addComponent<Component::MeshRenderer>(e, getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
				else
					mesh = addComponent<Component::MeshRenderer>(e, getInstance<AssetsManager>()->get<ObjFile>("obj__cube"));
				mesh->setShader("MaterialBasic");
#else
				auto mesh = addComponent<Component::MeshRenderer>(e, getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
				mesh->setShader("MaterialBasic");
#endif

#endif
			}
#ifdef LOG_FRAMERATE

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
private:
	std::size_t _frameCounter = 0;
	double _timeCounter = 0.0;
	double _maxTime = 8.0f;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25f;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
};