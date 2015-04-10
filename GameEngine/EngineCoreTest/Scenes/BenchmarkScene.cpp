#include <Scenes/BenchmarkScene.hpp>
#include <Configuration.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Utils/MathematicTools.hh>
#include <Skinning/AnimationManager.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/Commands/MainToPrepareCommands.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <glm/glm.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Core/Inputs/Input.hh>

#include <Components/FreeFlyComponent.hh>
#include <Components/CameraComponent.hpp>
#include <Components/Light.hh>
#include <Components/RigidBody.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/Lifetime.hpp>
#include <Components/RotationComponent.hpp>

#include <Systems/RotationSystem.hpp>

#include <Systems/DebugSystem.hpp>

#include <Render/Program.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
# include <Render/ProgramResources/Types/UniformBlock.hh>
# include <Render/ProgramResources/Types/Attribute.hh>
# include <Render/GeometryManagement/Painting/Painter.hh>
# include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
# include <Render/GeometryManagement/Painting/PaintingManager.hh>

#include <EngineCoreTestConfiguration.hpp>

#include <Skinning/Skeleton.hpp>
#include <Utils/MatrixConversion.hpp>

#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>

#include <Systems/FreeFlyCamera.hh>
#include <Systems/PhysicsSystem.hpp>

namespace AGE
{
	Key<AGE::AnimationInstance> animationTestInstance;
	std::vector<Entity> bonesEntities;


	BenchmarkScene::BenchmarkScene(AGE::Engine *engine)
		: AScene(engine)
	{

	}

	BenchmarkScene::~BenchmarkScene(void)
	{
	}

	void BenchmarkScene::initRendering()
	{
	}

	bool BenchmarkScene::_userStart()
	{
		EngineCoreTestConfiguration::RefreshScenesDirectoryListing();

		// We register component types so that we can load components from file
		// It'll create the component manager for the scene and
		// register the type in the global component register manager
		REGISTER_COMPONENT_TYPE(AGE::CameraComponent);
		REGISTER_COMPONENT_TYPE(AGE::MeshRenderer);
		REGISTER_COMPONENT_TYPE(AGE::Lifetime);
		REGISTER_COMPONENT_TYPE(AGE::RigidBody);
		REGISTER_COMPONENT_TYPE(AGE::PointLightComponent);
		REGISTER_COMPONENT_TYPE(AGE::FreeFlyComponent);
		REGISTER_COMPONENT_TYPE(AGE::RotationComponent);

		setInstance<AGE::BulletDynamicManager, AGE::BulletCollisionManager>()->init();
		addSystem<AGE::DebugSystem>(0);
		addSystem<AGE::PhysicsSystem>(0, Physics::EngineTypes::PhysX);
		deleteSystem<AGE::PhysicsSystem>();
		
		// TODO: Remove following line
		addSystem<AGE::BulletDynamicSystem>(0);

		addSystem<AGE::LifetimeSystem>(2);
		addSystem<AGE::FreeFlyCamera>(0);
		addSystem<AGE::RotationSystem>(0);

		getInstance<AGE::AssetsManager>()->setAssetsDirectory(EngineCoreTestConfiguration::GetCookedDirectory());
		getInstance<AGE::BulletCollisionManager>()->setAssetsDirectory(EngineCoreTestConfiguration::GetCookedDirectory());

		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("cube/cube.sage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("ball/ball.sage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("cube/cube.mage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("ball/ball.mage"), "DEMO_SCENE_BASIC_ASSETS");

		srand(42);

		return true;
	}

	bool BenchmarkScene::_userUpdateBegin(float time)
	{
		++_chunkFrame;
		_chunkCounter += time;

		if (getInstance<AGE::AssetsManager>()->isLoading())
		{
			return true;
		}

		if (this->getNumberOfEntities() == 0
			|| ImGui::ListBox("Scenes"
			, &EngineCoreTestConfiguration::getSelectedSceneIndex()
			, EngineCoreTestConfiguration::getScenesName().data()
			, EngineCoreTestConfiguration::getScenesName().size()))
		{
			clearAllEntities();

			auto camera = createEntity();
			GLOBAL_CAMERA = camera;
			auto cam = camera.addComponent<CameraComponent>();
			camera.addComponent<FreeFlyComponent>();
			cam->addPipeline(RenderType::DEFERRED);
			camera.getLink().setPosition(glm::vec3(0, 2.5f, 4.5f));

			auto sceneFileName = EngineCoreTestConfiguration::getSelectedScenePath() + "_export.json";
			auto assetPackageFileName = EngineCoreTestConfiguration::getSelectedScenePath() + "_assets.json";

			getInstance<AssetsManager>()->pushNewCallback(assetPackageFileName, std::function<void()>([=](){
				loadFromJson(sceneFileName);
			}));
			getInstance<AssetsManager>()->loadPackage(assetPackageFileName, assetPackageFileName);
		}

		if (getInstance<Input>()->getPhysicalKeyJustReleased(AGE_ESCAPE))
			return (false);

		static float trigger = 0.0f;
		if (getInstance<Input>()->getPhysicalKeyPressed(AGE_SPACE) && trigger == 0.0f)
		{
			trigger += time;
			if (trigger >= 1.0f)
				trigger = 0;
			auto e = createEntity();
			e.addComponent<Lifetime>(30.0f);
			auto &link = e.getLink();
			link.setPosition(GLOBAL_CAMERA.getLink().getPosition() + glm::vec3(0, 0, -2) * GLOBAL_CAMERA.getLink().getOrientation());
			link.setScale(glm::vec3(0.2f));
			MeshRenderer *mesh;
			mesh = e.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage")
				, getInstance<AGE::AssetsManager>()->getMaterial(OldFile("cube/cube.mage")));
			mesh->enableMode(RenderModes::AGE_OPAQUE);
			auto rigidBody = e.addComponent<RigidBody>(1.0f);
			rigidBody->setCollisionShape(RigidBody::BOX);
			rigidBody->getBody().setFriction(0.5f);
			rigidBody->getBody().setRestitution(0.5f);
			rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(GLOBAL_CAMERA.getLink().getOrientation() * glm::vec3(0, 0, -10)));
		}
		else
			trigger = 0.0f;

		if (_chunkCounter >= _maxChunk)
		{
			for (auto i = 0; i < 10; ++i)
			{
				auto e = createEntity();
				e.addComponent<Lifetime>(5.0f);

				auto &link = e.getLink();
				link.setPosition(glm::vec3((rand() % 100) - 50, (rand() % 50) - 5, (rand() % 100) - 50));
				link.setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
				link.setScale(glm::vec3(1.0f));


				MeshRenderer *mesh;
				if (i % 4 == 0)
				{
					mesh = e.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"), getInstance<AGE::AssetsManager>()->getMaterial(OldFile("ball/ball.mage")));
					link.setScale(glm::vec3(0.5f));
				}
				else
				{
					mesh = e.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"), getInstance<AGE::AssetsManager>()->getMaterial(OldFile("cube/cube.mage")));
				}

				if (i % 13 == 0)
				{
					e.addComponent<PointLightComponent>()->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.005f));
				}

				mesh->enableMode(RenderModes::AGE_OPAQUE);

				auto rigidBody = e.addComponent<RigidBody>(1.0f);
				if (i % 4 == 0)
					rigidBody->setCollisionShape(RigidBody::SPHERE);
				else
					rigidBody->setCollisionShape(RigidBody::BOX);
				rigidBody->getBody().setFriction(0.5f);
				rigidBody->getBody().setRestitution(0.5f);
				rigidBody->getBody().applyTorque(btVector3(float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f));
			}
			_chunkCounter = 0;
		}

		if (ImGui::Button("Reload shaders or type R") || getInstance<Input>()->getPhysicalKeyPressed(AGE_r))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ReloadShaders>();
		}

		auto camComponent = GLOBAL_CAMERA.getComponent<CameraComponent>();
		static bool cameraPipelines[2] = {false, false};
		cameraPipelines[RenderType::DEFERRED] = camComponent->havePipeline(RenderType::DEFERRED);

		if (ImGui::Checkbox("Deferred rendering", &cameraPipelines[RenderType::DEFERRED]))
		{
			if (cameraPipelines[RenderType::DEFERRED])
			{
				camComponent->addPipeline(RenderType::DEFERRED);
			}
			else
			{
				camComponent->removePipeline(RenderType::DEFERRED);
			}
		}

		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::RenderDrawLists>();
		return true;
	}


	bool BenchmarkScene::_userUpdateEnd(float time)
	{
		return true;
	}
}