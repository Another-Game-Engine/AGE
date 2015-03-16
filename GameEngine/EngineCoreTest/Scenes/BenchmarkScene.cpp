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

#include <Systems/DebugSystem.hpp>

#include <Render/Program.hh>
//#include <Render/GeometryManagement/Vertices.hh>
//#include <Render/GeometryManagement/BufferPrograms.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
//#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
# include <Render/ProgramResources/Types/UniformBlock.hh>
# include <Render/ProgramResources/Types/Attribute.hh>
# include <Render/GeometryManagement/Painting/Painter.hh>
# include <Render/Pipelining/Render/RenderingPass.hh>
# include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
# include <Render/GeometryManagement/Painting/PaintingManager.hh>

#include <Skinning/Skeleton.hpp>
#include <Utils/MatrixConversion.hpp>

#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>

#include <Systems/FreeFlyCamera.hh>

namespace AGE
{
	bool BenchmarkScene::initRenderingJustOneTime = true;

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

	bool BenchmarkScene::userStart()
	{
		// We register component types so that we can load components from file
		// It'll create the component manager for the scene and
		// register the type in the global component register manager
		REGISTER_COMPONENT_TYPE(AGE::CameraComponent);
		REGISTER_COMPONENT_TYPE(AGE::MeshRenderer);
		REGISTER_COMPONENT_TYPE(AGE::Lifetime);
		REGISTER_COMPONENT_TYPE(AGE::RigidBody);
		REGISTER_COMPONENT_TYPE(AGE::PointLightComponent);

#ifdef PHYSIC_SIMULATION
		setInstance<AGE::BulletDynamicManager, AGE::BulletCollisionManager>()->init();

#endif

		addSystem<AGE::DebugSystem>(0);

#ifdef PHYSIC_SIMULATION
		addSystem<AGE::BulletDynamicSystem>(0);
		//		addSystem<CollisionAdder>(1);
		//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

		if (initRenderingJustOneTime)
		{
			initRendering();
			initRenderingJustOneTime = false;
		}

		getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/Serialized/");
		getInstance<AGE::BulletCollisionManager>()->setAssetsDirectory("../../Assets/Serialized/");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("catwoman/catwoman.sage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("cube/cube.sage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("ball/ball.sage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("Sponza/sponza.sage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("cube/cube.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("ball/ball.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("catwoman/catwoman.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("Sponza/sponza.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadSkeleton(OldFile("catwoman/catwoman.skage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadAnimation(OldFile("catwoman/catwoman.aage"), "DEMO_SCENE_ASSETS");

#ifdef LIFETIME_ACTIVATED
		addSystem<AGE::LifetimeSystem>(2);
#endif //!LIFETIME_ACTIVATED

		addSystem<AGE::FreeFlyCamera>(0);

		srand(42);
		return true;
	}

	bool BenchmarkScene::userUpdateBegin(float time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;

		int totalToLoad = 0;
		int toLoad = 0;
		std::string loadingError;
		getInstance<AGE::AssetsManager>()->updateLoadingChannel("DEMO_SCENE_ASSETS", totalToLoad, toLoad, loadingError);
		if (loadingError.size() != 0)
			std::cout << loadingError << std::endl;
		if (toLoad > 0)
		{
			if (!ImGui::Begin("ASSETS LOADING", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
			{
				ImGui::End();
			}
			else
			{
				ImGui::SetWindowPos(ImVec2(getInstance<IRenderContext>()->getScreenSize().x / 2, getInstance<IRenderContext>()->getScreenSize().y / 2));
				ImGui::Text("Assets loading : %s / %s", std::to_string(toLoad).c_str(), std::to_string(totalToLoad).c_str());
				ImGui::End();
			}
			return true;
		}
		else
		{
			static bool init = true;
			if (init)
			{
				init = false;
				auto camera = createEntity();
				GLOBAL_CAMERA = camera;
				auto cam = camera.addComponent<CameraComponent>();
				camera.addComponent<FreeFlyComponent>();
				cam->addPipeline(RenderType::DEFERRED);

				auto screeSize = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Render::GetWindowSize, glm::uvec2>().get();

				auto &camLink = camera.getLink();
				camLink.setPosition(glm::vec3(0, 1.5, 0));

				GLOBAL_FLOOR = createEntity();
				auto &link = GLOBAL_FLOOR.getLink();
				link.setPosition(glm::vec3(0, -0.532, 0));
				link.setScale(glm::vec3(100, 1, 100));
				auto mesh = GLOBAL_FLOOR.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"), getInstance<AGE::AssetsManager>()->getMaterial("cube/cube.mage"));
				{
					GLOBAL_SPONZA = createEntity();
					auto& _l = GLOBAL_SPONZA.getLink();
					RigidBody *rb = GLOBAL_SPONZA.addComponent<RigidBody>(0.0f);
					rb->setCollisionMesh("sponza/sponza_static.phage");
					_l.setScale(glm::vec3(10.f));
				
					GLOBAL_SPONZA.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("Sponza/sponza.sage")
						, getInstance<AGE::AssetsManager>()->getMaterial("Sponza/sponza.mage"));
				}

	//{
	//	GLOBAL_CATWOMAN = createEntity();
	//	auto &_l = GLOBAL_CATWOMAN.getLink();

	//	static bool useOnce = false;
	//	auto rigidBody = GLOBAL_CATWOMAN.addComponent<RigidBody>(0);
	//	rigidBody->setCollisionMesh("catwoman/catwoman_dynamic.phage");
	//	//_l.setOrientation(glm::quat(glm::vec3(Mathematic::degreeToRadian(-90), Mathematic::degreeToRadian(90), 0)));
	//	_l.setPosition(glm::vec3(-30, 0, 0));
	//	_l.setScale(glm::vec3(0.01f));
	//	auto _m = GLOBAL_CATWOMAN.addComponent<MeshRenderer>(
	//		getInstance<AGE::AssetsManager>()->getMesh("catwoman/catwoman.sage")
	//		, getInstance<AGE::AssetsManager>()->getMaterial("catwoman/catwoman.mage"));
	//}

	for (int i = 0; i < GLOBAL_LIGHTS.size(); ++i)
	{
		GLOBAL_LIGHTS[i] = createEntity();
		auto e = GLOBAL_LIGHTS[i];
		auto &_l = e.getLink();
		_l.setPosition(glm::vec3(i, 1.0f, i));
		_l.setScale(glm::vec3(0.05f));
		auto _m = e.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"), getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
		e.getLink().setPosition(glm::vec3(i, 5.0f, 0));
		e.addComponent<PointLightComponent>()->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 5.0f, 0));
	}

#ifdef PHYSIC_SIMULATION
	auto rigidBody = GLOBAL_FLOOR.addComponent<RigidBody>(0.0f);
	rigidBody->setCollisionShape(RigidBody::BOX);
	rigidBody->getBody().setFriction(0.3f);
#endif //PHYSIC_SIMULATION


	////////////////////////////////////

	setInstance<AGE::AnimationManager>();
	auto skeleton = getInstance<AssetsManager>()->getSkeleton("catwoman/catwoman.skage");
	auto animation = getInstance<AssetsManager>()->getAnimation("catwoman/catwoman.aage");

	animationTestInstance = getInstance<AGE::AnimationManager>()->createAnimationInstance(skeleton, animation);

	auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
	
	//auto i = 0;
	//for (auto &e : skeleton->bones)
	//{
	//	auto entity = createEntity();
	//	auto child = createEntity();
	//	child.addComponent<MeshRenderer>(
	//		getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage")
	//		, getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
	//	child.getLink().setScale(1.0f);
	//	entity.getLink().setPosition(posFromMat4(e.transformation));
	//	entity.getLink().attachChild(child.getLinkPtr());
	//	bonesEntities.push_back(entity);
	//}

	////////////////////////////////////


	return true;
			}
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
#ifdef LIFETIME_ACTIVATED
				e.addComponent<Lifetime>(5.0f);
#endif

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

#ifdef PHYSIC_SIMULATION
				auto rigidBody = e.addComponent<RigidBody>(1.0f);
				if (i % 4 == 0)
					rigidBody->setCollisionShape(RigidBody::SPHERE);
				else
					rigidBody->setCollisionShape(RigidBody::BOX);
				rigidBody->getBody().setFriction(0.5f);
				rigidBody->getBody().setRestitution(0.5f);
				rigidBody->getBody().applyTorque(btVector3(float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f));
#endif
			}
			_chunkCounter = 0;
		}

#ifdef USE_IMGUI
		if (ImGui::CollapsingHeader("Light settings"))
		{
			for (int i = 0; i < GLOBAL_LIGHTS.size(); ++i)
			{
				auto e = GLOBAL_LIGHTS[i];
				auto &link = e.getLink();

				if (ImGui::SliderFloat3(std::string("Light " + std::to_string(i) + " position").c_str(), link.getPositionPtr(), -30, 30))
				{
					auto &l = e.getLink();
					l.setPosition(l.getPosition());
				}
				auto lightComponent = e.getComponent<AGE::PointLightComponent>();
				if (ImGui::ColorEdit3(std::string("Light " + std::to_string(i) + " color").c_str(), lightComponent->getColorPtr()))
				{
					lightComponent->set(lightComponent->getColor(), lightComponent->getRange());
				}
				if (ImGui::SliderFloat3(std::string("Light " + std::to_string(i) + " attenuation").c_str(), lightComponent->getRangePtr(), 0.001, 1.0))
				{
					lightComponent->set(lightComponent->getColor(), lightComponent->getRange());
				}
			}
		}
#endif

		if (ImGui::Button("Reload shaders or type R") || getInstance<Input>()->getPhysicalKeyPressed(AGE_r))
		{
			GetRenderThread()->getQueue()->emplaceTask<Tasks::Render::ReloadShaders>();
		}

		auto camComponent = GLOBAL_CAMERA.getComponent<CameraComponent>();
		static bool cameraPipelines[2] = {false, false};
		cameraPipelines[RenderType::BASIC] = camComponent->havePipeline(RenderType::BASIC);
		cameraPipelines[RenderType::DEFERRED] = camComponent->havePipeline(RenderType::DEFERRED);

		if (ImGui::Checkbox("Basic rendering", &cameraPipelines[RenderType::BASIC]))
		{
			if (cameraPipelines[RenderType::BASIC])
			{
				camComponent->addPipeline(RenderType::BASIC);
			}
			else
			{
				camComponent->removePipeline(RenderType::BASIC);
			}
		}
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

		// TODO
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
		// TODO
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::RenderDrawLists>();

		//static float ttime = 0;
		//static float timeMultiplier = 0.0f;
		//ImGui::SliderFloat("Animation time", &timeMultiplier, 0.0f, 200.0f);
		//getInstance<AGE::AnimationManager>()->update(timeMultiplier);
		//ttime += time;
		//auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
		//auto skeleton = getInstance<AssetsManager>()->getSkeleton("catwoman/catwoman.skage");
		//skeleton->updateSkinning();
		//for (std::size_t i = 0; i < bones.size(); ++i)
		//{
		////	bones[i] = glm::mat4(1);
		//	//bonesEntities[i].getLink().setPosition(posFromMat4(bones[i]));
		//}
		//DirtyBoneContainer::setBones(bones);
		return true;
	}


	bool BenchmarkScene::userUpdateEnd(float time)
	{
		return true;
	}
}