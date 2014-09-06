#pragma once

#include <Core/AScene.hh>

#include <Systems/LifetimeSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/CameraSystem.hh>
#include <Components\/MeshRenderer.hh>
#include <Components/PointLight.hh>

#include <Core/AssetsManager.hpp>

#include <Core/Octree.hpp>

#include <Context/IRenderContext.hh>

#include <CONFIGS.hpp>

//for test
#include <Utils/Containers/CommandQueue.hpp>

Entity GLOBAL_CAMERA;

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
		std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
		getInstance<AGE::Octree>()->setScene(weakOnThis);

#ifdef PHYSIC_SIMULATION
		addSystem<BulletDynamicSystem>(0);
//		addSystem<CollisionAdder>(1);
//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

#ifdef RENDERING_ACTIVATED

		auto &camerasystem = addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	auto &m = *getInstance<gl::RenderManager>();
#if NEW_SHADER
	camerasystem->setManager(m);
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
		GLOBAL_CAMERA = camera;
		auto cam = addComponent<Component::CameraComponent>(camera);

		auto screenSize = getInstance<IRenderContext>()->getScreenSize();
		cam->fboSize = screenSize;
		cam->viewport = glm::uvec4(0, 0, cam->fboSize.x, cam->fboSize.y);
		cam->sampleNbr = 0;

		auto camLink = getLink(camera);
		camLink->setPosition(glm::vec3(0, 0, -10));

	auto light = createEntity();
	auto lightLink = getLink(light);
	auto lightComponent = addComponent<Component::PointLight>(light);
	lightComponent->lightData.colorRange = glm::vec4(1, 1, 1, 30);
	lightComponent->lightData.positionPower.w = 0.5f;
	lightComponent->lightData.hasShadow = -1;
	lightLink->setPosition(glm::vec3(0, 3, 0));

	auto plane = createEntity();
	auto link = getLink(plane);
	link->setPosition(glm::vec3(0, -10, 0));
	link->setScale(glm::vec3(100, 1, 100));
	auto mesh = addComponent<Component::MeshRenderer>(plane, getInstance<AGE::AssetsManager>()->loadMesh("cube/cube.sage"));
	mesh->setMaterial(getInstance<AGE::AssetsManager>()->loadMaterial(File("cube/cube.mage")));
#ifdef PHYSIC_SIMULATION
	auto rigidBody = addComponent<Component::RigidBody>(plane, 0.0f);
	rigidBody->setCollisionShape(weakOnThis, plane, Component::RigidBody::BOX);
	rigidBody->getBody().setFriction(0.8f);
#endif //PHYSIC_SIMULATION
#endif

	return true;
}

	virtual bool 			userUpdate(double time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;

		getLink(GLOBAL_CAMERA)->setOrientation(glm::rotate(getLink(GLOBAL_CAMERA)->getOrientation(), 100.0f * (float)time, glm::vec3(0, 1, 0)));

		if (_chunkCounter >= _maxChunk)
		{
			std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
			for (auto i = 0; i < 30; ++i)
			{
				auto e = createEntity();

#ifdef LIFETIME_ACTIVATED
				addComponent<Component::Lifetime>(e, 5.0f);
#endif
#ifdef RENDERING_ACTIVATED

				if (i == 0)
				{
					auto lightComponent = addComponent<Component::PointLight>(e);
					lightComponent->lightData.colorRange = glm::vec4((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 13);
					lightComponent->lightData.positionPower.w = 1;
					lightComponent->lightData.hasShadow = 1;
				}

#ifndef COMPLEX_MESH
				Component::MeshRenderer *mesh;
				if (i % 4 == 0)
				{
					mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->loadMesh("ball/ball.sage"));
					mesh->setMaterial(getInstance<AGE::AssetsManager>()->loadMaterial(File("ball/ball.mage")));
					for (size_t index = 0; index < mesh->getMaterial()->datas.size(); ++index)
					{
						gl::Key<gl::Material> mat = mesh->getMaterial()->datas[index];
						getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Color_diffuse>(mat, glm::vec4((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1));
						getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Ratio_diffuse>(mat, 1.0f);
					}
				}
				else
				{
					mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->loadMesh("cube/cube.sage"));
					mesh->setMaterial(getInstance<AGE::AssetsManager>()->loadMaterial(File("cube/cube.mage")));
					for (size_t index = 0; index < mesh->getMaterial()->datas.size(); ++index)
					{
						gl::Key<gl::Material> mat = mesh->getMaterial()->datas[index];
						getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Color_diffuse>(mat, glm::vec4(0, 0.0f, 1.f, 0.f));
						getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Ratio_diffuse>(mat, (float)(rand() % 100) / 100.0f);
					}
				}
#else
				auto mesh = addComponent<Component::MeshRenderer>(e, getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
				mesh->setShader("MaterialBasic");
#endif

#endif

				auto link = getLink(e);
				link->setPosition(glm::vec3((rand() % 100) - 50, (rand() % 20) - 5, (rand() % 100) - 50));
				link->setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
				link->setScale(glm::vec3(3.0f));

#ifdef PHYSIC_SIMULATION
				auto rigidBody = addComponent<Component::RigidBody>(e, 1.0f);
//				rigidBody->setTransformation(link->getTransform());
				if (i % 4 == 0)
					rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::SPHERE);
				else
					rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::BOX);
				rigidBody->getBody().setFriction(0.5f);
				rigidBody->getBody().setRestitution(0.5f);
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
	double _maxTime = 150.0f;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25f;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
};