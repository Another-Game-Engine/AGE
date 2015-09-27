#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

#include <Scenes/BenchmarkScene.hpp>
#include <Configuration.hpp>
#include <Core/Age_Imgui.hpp>
#include <Utils/MathematicTools.hh>
#include <Skinning/AnimationManager.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Commands/MainToPrepareCommands.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <glm/glm.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Core/Inputs/Input.hh>

#include <Configuration.hpp>

#include <ComponentsCore/FreeFlyComponent.hh>
#include <ComponentsCore/CameraComponent.hpp>
#include <ComponentsCore/Light.hh>
#include <ComponentsCore/SpotLight.hh>
#include <ComponentsCore/RigidBody.hpp>
#include <ComponentsCore/MeshRenderer.hh>
#include <Components/Lifetime.hpp>
#include <Components/RotationComponent.hpp>
#include <ComponentsCore/Collider.hpp>
#include <Components/ArchetypeComponent.hpp>
#include <ComponentsCore/CharacterController.hh>
#include <ComponentsCore/PhysicsData.hpp>
#include <ComponentsCore/DirectionalLightComponent.hh>

#include <Components/ComponentRegistrationManager.hpp>

#include <Systems/RotationSystem.hpp>
#include <SystemsCore/DebugSystem.hpp>
#include <SystemsCore/RenderCameraSystem.hpp>
#include <SystemsCore/CharacterControllerSystem.hh>
#include <SystemsCore/FPSCharacterSystem.hh>
#include <Systems/BasicDemoCameraSystem.hh>

#include <Render/Program.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <Render/ProgramResources/Types/Attribute.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <render/OcclusionTools/OcclusionOptions.hpp>

#include <EngineCoreTestConfiguration.hpp>

#include <Skinning/Skeleton.hpp>
#include <Utils/MatrixConversion.hpp>

#include <SystemsCore/FreeFlyCamera.hh>


#include <SystemsCore/PhysicsSystem.hpp>
#include <SystemsCore/PhysicsDebugSystem.hpp>

#include <Graphic/DRBCameraDrawableList.hpp>
#include <Graphic/BFCCullableTypes.hpp>
#include <BFC/BFCBlockManagerFactory.hpp>
#include <BFC/BFCLinkTracker.hpp>

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
		REGISTER_COMPONENT_TYPE(AGE::CharacterController);

		getInstance<AGE::AssetsManager>()->setAssetsDirectory(EngineCoreTestConfiguration::GetCookedDirectory());

		addSystem<AGE::BasicDemoCameraSystem>(0);

		addSystem<AGE::DebugSystem>(0);
		addSystem<AGE::PhysicsSystem>(1, Physics::EngineType::PhysX, getInstance<AGE::AssetsManager>());
		addSystem<AGE::CharacterControllerSystem>(0, false);
		addSystem<AGE::FPSCharacterSystem>(0);

		addSystem<AGE::LifetimeSystem>(2);
		addSystem<AGE::FreeFlyCamera>(0);
		addSystem<AGE::RotationSystem>(0);
		addSystem<AGE::RenderCameraSystem>(1000000);

		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("cube/cube.sage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("ball/ball.sage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("cube/cube.mage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("ball/ball.mage"), "DEMO_SCENE_BASIC_ASSETS");
//		_skyboxTest = getInstance<AGE::AssetsManager>()->loadSkybox("test", OldFile("skyboxes/test.dds"), "DEMO_SCENE_BASIC_ASSETS");
		_skyboxSpace = getInstance<AGE::AssetsManager>()->loadCubeMap("space", OldFile("skyboxes/space.dds"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadAnimation(OldFile("hexapod/animation/run.aage"), "DEMO_SCENE_BASIC_ASSETS");
		getInstance<AGE::AssetsManager>()->loadSkeleton(OldFile("hexapod/animation/run.skage"), "DEMO_SCENE_BASIC_ASSETS");

		setInstance<AGE::AnimationManager>();
 
		srand(42);

		return true;
	}

	bool BenchmarkScene::_userUpdateBegin(float time)
	{
#ifdef VISUAL_LEAK_DETECTOR
//		int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//		tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
//		_CrtSetDbgFlag(tmpFlag);
		assert(_CrtCheckMemory());
#endif

		++_chunkFrame;
		_chunkCounter += time;

		if (getInstance<AGE::AssetsManager>()->isLoading())
		{
			return true;
		}
		static int toto = 0;
		++toto;
		if (this->getNumberOfEntities() == 0 && toto > 10
#if defined(AGE_ENABLE_IMGUI)
			|| ImGui::ListBox("Scenes"
			, &EngineCoreTestConfiguration::getSelectedSceneIndex()
			, EngineCoreTestConfiguration::getScenesName().data()
			, static_cast<int>(EngineCoreTestConfiguration::getScenesName().size())) && toto > 10
#endif
			)
		{
			EngineCoreTestConfiguration::saveConfigurations();
			clearAllEntities();

//			auto camera = createEntity();
//			GLOBAL_CAMERA = camera;
//			auto cam = camera->addComponent<CameraComponent>();
//			camera->addComponent<FreeFlyComponent>();
//			cam->setPipeline(RenderType::DEFERRED);
//			getSystem<RenderCameraSystem>()->drawDebugLines(false);
//			cam->setTexture(_skyboxSpace);
//			cam->addSkyBoxToChoice("space", _skyboxSpace);
////			cam->addSkyBoxToChoice("test", _skyboxTest);
//			camera->getLink().setPosition(glm::vec3(0, 2.5f, 4.5f));

			auto sceneFileName = EngineCoreTestConfiguration::getSelectedScenePath();

			if (!sceneFileName.empty())
			{
				load(sceneFileName);
			}


			auto skeleton = getInstance<AssetsManager>()->getSkeleton("hexapod/animation/run.skage");
			auto animation = getInstance<AssetsManager>()->getAnimation("hexapod/animation/run.aage");

			animationTestInstance = getInstance<AGE::AnimationManager>()->createAnimationInstance(skeleton, animation);
			auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
			for (auto &e : bones)
			{
				auto entity = createEntity();
				entity->addComponent<MeshRenderer>(
					getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage")
					, getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
				bonesEntities.push_back(entity);
				entity->getLink().setPosition(posFromMat4(e));
				entity->getLink().setScale(0.1f);
			}

		}

		if (getInstance<Input>()->getPhysicalKeyJustReleased(AgeKeys::AGE_ESCAPE))
			return (false);


		if (getNumberOfEntities() == 0)
		{
			return true;
		}

		static bool rain = false;

#if defined(AGE_ENABLE_IMGUI)
		ImGui::Checkbox("Cube rain", &rain);
		
		static bool physicsDebug = false;
		bool lastValue = physicsDebug;
		ImGui::Checkbox("Physics Debug", &physicsDebug);
		if (physicsDebug != lastValue)
		{
			if (physicsDebug)
			{
				addSystem<Private::PhysicsDebugSystem>(5);
			}
			else
			{
				deleteSystem<Private::PhysicsDebugSystem>();
			}
		}

		ImGui::Checkbox("Occlusion culling", &AGE::OcclusionConfig::g_Occlusion_is_enabled);
#endif

		if (rain && _chunkCounter >= _maxChunk)
		{
			for (auto i = 0; i < 10; ++i)
			{
				auto e = createEntity();
				e->addComponent<Lifetime>(5.0f);

				auto &link = e->getLink();
				link.setPosition(glm::vec3((rand() % 100) - 50, (rand() % 50) + 50, (rand() % 100) - 50));
				link.setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
				link.setScale(glm::vec3((float)(rand() % 30) / 10.0f));


				MeshRenderer *mesh;
				if (i % 4 == 0)
				{
					mesh = e->addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"), getInstance<AGE::AssetsManager>()->getMaterial(OldFile("ball/ball.mage")));
					e->addComponent<Collider>(Physics::ColliderType::Mesh, "ball/ball");
				}
				else
				{
					mesh = e->addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"), getInstance<AGE::AssetsManager>()->getMaterial(OldFile("cube/cube.mage")));
					e->addComponent<Collider>(Physics::ColliderType::Mesh, "cube/cube");
				}

				if (i % 20 == 0)
				{
					auto pl = e->addComponent<PointLightComponent>();
					pl->setColor(glm::vec3(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f));
				}
				e->addComponent<RigidBody>();
				mesh->enableRenderMode(RenderModes::AGE_OPAQUE);
			}
			_chunkCounter = 0;
		}
#if defined(AGE_ENABLE_IMGUI)
		if (ImGui::Button("Reload shaders or type R") || getInstance<Input>()->getPhysicalKeyPressed(AgeKeys::AGE_r))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ReloadShaders>();
		}

		static float tt = 0;
		tt += time;
		getInstance<AGE::AnimationManager>()->update(tt);
		auto skel = getInstance<AssetsManager>()->getSkeleton("hexapod/animation/run.skage");
		skel->updateSkinning();

		int i = 0;
		auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
		for (auto &e : bones)
		{
			bonesEntities[i]->getLink().setPosition(posFromMat4(e));
			++i;
		}

		ImGui::Text("%i entities", getNumberOfEntities());
#endif
		return true;
	}


	bool BenchmarkScene::_userUpdateEnd(float time)
	{
#ifdef AGE_BFC

#endif
		return true;
	}
}