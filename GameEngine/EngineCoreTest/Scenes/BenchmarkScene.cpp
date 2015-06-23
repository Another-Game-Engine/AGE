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
#include <Components/SpotLight.hh>
#include <Components/RigidBody.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/Lifetime.hpp>
#include <Components/RotationComponent.hpp>
#include <Components/Collider.hpp>
#include <Components/ArchetypeComponent.hpp>

#include <Components/ComponentRegistrationManager.hpp>

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
		REGISTER_COMPONENT_TYPE(AGE::DirectionalLightComponent);
		REGISTER_COMPONENT_TYPE(AGE::SpotLightComponent);
		REGISTER_COMPONENT_TYPE(AGE::FreeFlyComponent);
		REGISTER_COMPONENT_TYPE(AGE::RotationComponent);
		REGISTER_COMPONENT_TYPE(AGE::Collider);
		REGISTER_COMPONENT_TYPE(AGE::DirectionalLightComponent);
		REGISTER_COMPONENT_TYPE(AGE::ArchetypeComponent);
		REGISTER_COMPONENT_TYPE(AGE::RigidBody);


		addSystem<AGE::DebugSystem>(0);
		addSystem<AGE::PhysicsSystem>(0, Physics::EngineType::PhysX, EngineCoreTestConfiguration::GetCookedDirectory());

		addSystem<AGE::LifetimeSystem>(2);
		addSystem<AGE::FreeFlyCamera>(0);
		addSystem<AGE::RotationSystem>(0);

		getInstance<AGE::AssetsManager>()->setAssetsDirectory(EngineCoreTestConfiguration::GetCookedDirectory());

		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("cube/cube.sage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("ball/ball.sage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("cube/cube.mage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("ball/ball.mage"), "DEMO_SCENE_BASIC_ASSETS");
//		_skyboxTest = getInstance<AGE::AssetsManager>()->loadSkybox("test", OldFile("skyboxes/test.dds"), "DEMO_SCENE_BASIC_ASSETS");
		_skyboxSpace = getInstance<AGE::AssetsManager>()->loadCubeMap("space", OldFile("skyboxes/space.dds"), "DEMO_SCENE_BASIC_ASSETS");
		//getInstance<AGE::AssetsManager>()->loadAnimation(OldFile("hexapod/animation/hexapod@attack(1).aage"), "DEMO_SCENE_BASIC_ASSETS");
		//getInstance<AGE::AssetsManager>()->loadSkeleton(OldFile("hexapod/animation/hexapod@attack(1).skage"), "DEMO_SCENE_BASIC_ASSETS");

		setInstance<AGE::AnimationManager>();

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
		static int toto = 0;
		++toto;
		if (this->getNumberOfEntities() == 0 && toto > 10
			|| ImGui::ListBox("Scenes"
			, &EngineCoreTestConfiguration::getSelectedSceneIndex()
			, EngineCoreTestConfiguration::getScenesName().data()
			, static_cast<int>(EngineCoreTestConfiguration::getScenesName().size())) && toto > 10)
		{
			EngineCoreTestConfiguration::saveConfigurations();
			clearAllEntities();

			auto camera = createEntity();
			GLOBAL_CAMERA = camera;
			auto cam = camera->addComponent<CameraComponent>();
			camera->addComponent<FreeFlyComponent>();
			cam->setPipeline(RenderType::DEFERRED);
			cam->setTexture(_skyboxSpace);
			cam->addSkyBoxToChoice("space", _skyboxSpace);
//			cam->addSkyBoxToChoice("test", _skyboxTest);
			camera->getLink().setPosition(glm::vec3(0, 2.5f, 4.5f));

			auto sceneFileName = EngineCoreTestConfiguration::getSelectedScenePath();

			if (!sceneFileName.empty())
			{
				load(sceneFileName);
			}


			//auto skeleton = getInstance<AssetsManager>()->getSkeleton("hexapod/animation/hexapod@attack(1).skage");
			//auto animation = getInstance<AssetsManager>()->getAnimation("hexapod/animation/hexapod@attack(1).aage");

			//animationTestInstance = getInstance<AGE::AnimationManager>()->createAnimationInstance(skeleton, animation);

			//auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
			//for (auto &e : bones)
			//{
			//	auto entity = createEntity();
			//	entity->addComponent<MeshRenderer>(
			//		getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage")
			//		, getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
			//	bonesEntities.push_back(entity);
			//}

		}

		if (getInstance<Input>()->getPhysicalKeyJustReleased(AGE_ESCAPE))
			return (false);


		if (getNumberOfEntities() == 0)
		{
			return true;
		}

		static float trigger = 1.0f;
		if (getInstance<Input>()->getPhysicalKeyPressed(AGE_SPACE) && trigger >= 0.15f)
		{
			trigger = 0.0f;
			auto e = createEntity();
			e->addComponent<Lifetime>(15.0f);
			auto &link = e->getLink();
			auto &cameraLink = GLOBAL_CAMERA->getLink();
			const glm::quat &cameraOrientation = cameraLink.getOrientation();
			const glm::vec3 cameraForward = glm::vec3(glm::mat4(glm::toMat4(cameraOrientation) * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -1.0f)))[3]);
			link.setPosition(cameraLink.getPosition());
			link.setOrientation(cameraOrientation);
			link.setScale(glm::vec3(0.2f));
			e->addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"),
										 getInstance<AGE::AssetsManager>()->getMaterial(OldFile("cube/cube.mage")))->enableRenderMode(RenderModes::AGE_OPAQUE);
			e->addComponent<RigidBody>()->addForce(10.0f * cameraForward, Physics::ForceMode::Impulse);
			e->addComponent<Collider>(Physics::ColliderType::Box);
		}
		trigger += time;

		if (_chunkCounter >= _maxChunk)
		{
			for (auto i = 0; i < 10; ++i)
			{
				auto e = createEntity();
				e->addComponent<Lifetime>(5.0f);

				auto &link = e->getLink();
				link.setPosition(glm::vec3((rand() % 100) - 50, (rand() % 50) - 5, (rand() % 100) - 50));
				link.setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
				link.setScale(glm::vec3(1.0f));


				MeshRenderer *mesh;
				if (i % 4 == 0)
				{
					mesh = e->addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"), getInstance<AGE::AssetsManager>()->getMaterial(OldFile("ball/ball.mage")));
					e->addComponent<Collider>(Physics::ColliderType::Sphere);
					link.setScale(glm::vec3(0.5f));
				}
				else
				{
					mesh = e->addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"), getInstance<AGE::AssetsManager>()->getMaterial(OldFile("cube/cube.mage")));
					e->addComponent<Collider>(Physics::ColliderType::Box);
				}

				if (i % 13 == 0)
				{
					//e->addComponent<PointLightComponent>()->set(PointLightData(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.005f)));
				}
				e->addComponent<RigidBody>();
				mesh->enableRenderMode(RenderModes::AGE_OPAQUE);
			}
			_chunkCounter = 0;
		}

		if (ImGui::Button("Reload shaders or type R") || getInstance<Input>()->getPhysicalKeyPressed(AGE_r))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ReloadShaders>();
		}

		ImGui::Text("%i entities", getNumberOfEntities());

		GLOBAL_CAMERA->getComponent<CameraComponent>()->editorUpdate();
		////////////////////////////////////

		//static float ttime = 0;
		//getInstance<AGE::AnimationManager>()->update(ttime);
		//ttime += time;
		//auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
		//for (std::size_t i = 0; i < bones.size(); ++i)
		//{
		//	bonesEntities[i]->getLink().setTransform(bones[i]);
		//}

		////////////////////////////////////

		auto camComponent = GLOBAL_CAMERA->getComponent<CameraComponent>();
		static char const *pipelineNames[RenderType::TOTAL] = {"Debug deferred rendering", "Deferred rendering" };
		ImGui::ListBox("Pipelines", &pipelineIndex, pipelineNames, int(RenderType::TOTAL));
		if (camComponent->getPipeline() != (RenderType)pipelineIndex)
			camComponent->setPipeline((RenderType)pipelineIndex);
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::RenderDrawLists>();
		return true;
	}


	bool BenchmarkScene::_userUpdateEnd(float time)
	{
		return true;
	}
}