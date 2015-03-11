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

#include <Components/CameraComponent.hpp>
#include <Components/Light.hh>
#include <Components/RigidBody.hpp>
#include <Components/MeshRenderer.hh>

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
		auto assetsManager = getInstance<AGE::AssetsManager>();
		auto mainThread = getEngine();

		////TODO
		AGE::GetRenderThread()->getQueue()->emplaceTask<AGE::Tasks::Basic::VoidFunction>(std::function<void()>([&](){
			auto &painter_manager = AGE::GetRenderThread()->paintingManager;

			//// create the shader
			//key.getBuff.shader = _renderManager->addShader(DEFFERED_VERTEX_SHADER, DEFFERED_FRAG_SHADER);
			//key.Accum.shader = _renderManager->addShader(DEFFERED_VERTEX_SHADER_ACCUM, DEFFERED_FRAG_SHADER_ACCUM);
			//key.merge.shader = _renderManager->addShader(DEFERRED_VERTEX_SHADER_MERGE, DEFERRED_FRAG_SHADER_MERGE);
			//
			//// get from the shader the information key
			//key.global_state = _renderManager->addUniformBlock();
			//_renderManager->addShaderInterfaceBlock(key.getBuff.shader, "global_state", key.global_state);
			//_renderManager->addShaderInterfaceBlock(key.Accum.shader, "global_state", key.global_state);
			//
			//// bind the key on drawable info (material-transformation)
			//_renderManager->bindMaterialToShader<gl::Color_specular>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "specular_color", glm::vec4(1.0f)));
			//_renderManager->bindMaterialToShader<gl::Ratio_specular>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "specular_ratio", 1.0f));
			//_renderManager->bindMaterialToShader<gl::Shininess>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "shininess", 0.01f));
			//_renderManager->bindMaterialToShader<gl::Color_diffuse>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "diffuse_color", glm::vec4(1.0f)));
			//_renderManager->bindMaterialToShader<gl::Ratio_diffuse>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "diffuse_ratio", 0.0f));
			//_renderManager->bindMaterialToShader<gl::Texture_diffuse>(key.getBuff.shader, _renderManager->addShaderSampler(key.getBuff.shader, "diffuse_texture"));
			//_renderManager->bindMaterialToShader<gl::Texture_normal>(key.getBuff.shader, _renderManager->addShaderSampler(key.getBuff.shader, "normal_texture"));
			//_renderManager->bindTransformationToShader(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "model_matrix", glm::mat4(1.f)));
			//
			//// found uniform
			//key.Accum.position_light = _renderManager->addShaderUniform(key.Accum.shader, "position_light", glm::vec3(1.f));
			//key.Accum.range_light = _renderManager->addShaderUniform(key.Accum.shader, "attenuation_light", glm::vec3(1.f, 0.1f, 0.01f));
			//key.Accum.color_light = _renderManager->addShaderUniform(key.Accum.shader, "color_light", glm::vec3(1.0f));
			//key.Accum.depth_buffer = _renderManager->addShaderSampler(key.Accum.shader, "depth_buffer");
			//key.Accum.normal_buffer = _renderManager->addShaderSampler(key.Accum.shader, "normal_buffer");
			//key.Accum.specular_buffer = _renderManager->addShaderSampler(key.Accum.shader, "specular_buffer");
			//key.Accum.ambiant_color = _renderManager->addShaderUniform(key.Accum.shader, "ambiant_color", glm::vec3(0.01f, 0.01f, 0.01f));
			//key.merge.diffuse_buffer = _renderManager->addShaderSampler(key.merge.shader, "diffuse_buffer");
			//key.merge.light_buffer = _renderManager->addShaderSampler(key.merge.shader, "light_buffer");
			//
			//// create renderpass and set it
			//auto screenSize = getInstance<IRenderContext>()->getScreenSize();
			//key.getBuff.renderPass = _renderManager->addRenderPass(key.getBuff.shader, glm::ivec4(0, 0, screenSize.x, screenSize.y));
			//_renderManager->pushSetTestTaskRenderPass(key.getBuff.renderPass, false, false, true);
			//_renderManager->pushSetClearValueTaskRenderPass(key.getBuff.renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
			//_renderManager->pushClearTaskRenderPass(key.getBuff.renderPass, true, true, false);
			//_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT0);
			//_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT1);
			//_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT2);
			//_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT0, GL_RGBA8);
			//_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT1, GL_RGBA8);
			//_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT2, GL_RGBA8);
			//_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
			//_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 0, false);
			//_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 1, false);
			//_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 2, false);
			//_renderManager->pushSetCullFaceRenderPass(key.getBuff.renderPass, GL_BACK);
			//_renderManager->pushDrawTaskRenderBuffer(key.getBuff.renderPass);
			//
			//// create  clear renderPass
			//key.clean.emptyRenderPass = _renderManager->addEmptyRenderPass(glm::ivec4(0, 0, screenSize.x, screenSize.y));
			//_renderManager->createBufferSamplableEmptyRenderPass(key.clean.emptyRenderPass, GL_COLOR_ATTACHMENT0, GL_RGBA8);
			//_renderManager->pushSetClearValueTaskEmptyRenderPass(key.clean.emptyRenderPass, glm::vec4(0.f, 0.f, 0.f, 1.0f));
			//_renderManager->pushSetTestTaskEmptyRenderPass(key.clean.emptyRenderPass, false, false, false);
			//_renderManager->pushClearTaskEmptyRenderPass(key.clean.emptyRenderPass, true, false, false);
			//_renderManager->pushSetBlendStateTaskEmptyRenderPass(key.clean.emptyRenderPass, 0, false);
			//_renderManager->pushTargetEmptyRenderPass(key.clean.emptyRenderPass, GL_COLOR_ATTACHMENT0);
			//_renderManager->pushOwnTaskEmptyRenderPass(key.clean.emptyRenderPass, [](gl::LocationStorage &l){
			//	size_t nbrElement = l.getLocation<size_t>(0);
			//	l.setLocation(nbrElement + 1, size_t(0));
			//});
			//// create renderPostEffect
			//gl::RenderManager *r = _renderManager;
			//RenderKey *k = &key;
			//key.Accum.renderPostEffect = _renderManager->addRenderPostEffect(key.Accum.shader, glm::ivec4(0, 0, screenSize.x, screenSize.y));
			//_renderManager->pushSetTestTaskRenderPostEffect(key.Accum.renderPostEffect, false, false, false);
			//_renderManager->pushTargetRenderPostEffect(key.Accum.renderPostEffect, GL_COLOR_ATTACHMENT0);
			//_renderManager->pushSetBlendStateTaskRenderPostEffect(key.Accum.renderPostEffect, 0, true);
			//_renderManager->pushSetBlendEquationTaskRenderPostEffect(key.Accum.renderPostEffect, GL_FUNC_ADD);
			//_renderManager->pushSetBlendFuncTaskRenderPostEffect(key.Accum.renderPostEffect, GL_ONE, GL_ONE);
			//_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.normal_buffer, GL_COLOR_ATTACHMENT1, key.getBuff.renderPass);
			//_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.depth_buffer, GL_DEPTH_ATTACHMENT, key.getBuff.renderPass);
			//_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.specular_buffer, GL_COLOR_ATTACHMENT2, key.getBuff.renderPass);
			//_renderManager->useInputBufferRenderPostEffect(key.Accum.renderPostEffect, GL_COLOR_ATTACHMENT0, key.clean.emptyRenderPass);
			//_renderManager->createBufferNotSamplableRenderPostEffect(key.Accum.renderPostEffect, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
			//_renderManager->pushOwnTaskRenderPostEffect(key.Accum.renderPostEffect, [=](gl::LocationStorage &l)
			//{
			//	size_t nbrElement = l.getLocation<size_t>(0);
			//	size_t index = l.getLocation<size_t>(nbrElement + 1);
			//	AGE::PointLight pointLight = l.getLocation<AGE::PointLight>(index + 1);
			//	r->setShaderUniform(k->Accum.shader, k->Accum.position_light, pointLight.position);
			//	r->setShaderUniform(k->Accum.shader, k->Accum.range_light, pointLight.range);
			//	r->setShaderUniform(k->Accum.shader, k->Accum.color_light, pointLight.color);
			//	l.setLocation(nbrElement + 1, index + 1);
			//});
			//
			//// create merge
			//key.merge.renderPostEffect = _renderManager->addRenderPostEffect(key.merge.shader, glm::ivec4(0, 0, screenSize.x, screenSize.y));
			//_renderManager->pushInputRenderPostEffect(key.merge.renderPostEffect, key.merge.light_buffer, GL_COLOR_ATTACHMENT0, key.Accum.renderPostEffect);
			//_renderManager->pushInputRenderPostEffect(key.merge.renderPostEffect, key.merge.diffuse_buffer, GL_COLOR_ATTACHMENT0, key.getBuff.renderPass);
			//_renderManager->createBufferSamplableRenderPostEffect(key.merge.renderPostEffect, GL_COLOR_ATTACHMENT0, GL_RGBA8);
			//_renderManager->createBufferNotSamplableRenderPostEffect(key.merge.renderPostEffect, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
			//_renderManager->pushTargetRenderPostEffect(key.merge.renderPostEffect, GL_COLOR_ATTACHMENT0);
			//_renderManager->pushClearTaskRenderPostEffect(key.merge.renderPostEffect, true, false, false);
			//_renderManager->pushSetTestTaskRenderPostEffect(key.merge.renderPostEffect, true, false, false);
			//_renderManager->pushSetBlendStateTaskRenderPostEffect(key.merge.renderPostEffect, 0, false);
			//
			//key.merge.renderOnScreen = _renderManager->addRenderOnScreen(glm::ivec4(0, 0, screenSize.x, screenSize.y), key.merge.renderPostEffect);
			//_renderManager->pushClearTaskRenderOnScreen(key.merge.renderOnScreen, true, true, false);
			//_renderManager->pushSetTestTaskRenderOnScreen(key.merge.renderOnScreen, false, false, true);
			//_renderManager->pushSetClearValueTaskRenderOnScreen(key.merge.renderOnScreen, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			//
			//// create the pipeline and set it with both render element add before
			//key.getBuff.pipeline = _renderManager->addPipeline();
			//_renderManager->pushRenderPassPipeline(key.getBuff.pipeline, key.getBuff.renderPass);
			//
			//// create the pipeline for cleaning
			//key.clean.pipeline = _renderManager->addPipeline();
			//_renderManager->configPipeline(key.clean.pipeline, gl::DrawType::NONE_OBJECT);
			//_renderManager->pushEmptyRenderPassPipeline(key.clean.pipeline, key.clean.emptyRenderPass);
			//
			//// create the pipeline for accumulation
			//key.Accum.pipeline = _renderManager->addPipeline();
			//_renderManager->pushRenderPostEffectPipeline(key.Accum.pipeline, key.Accum.renderPostEffect);
			//_renderManager->configPipeline(key.Accum.pipeline, gl::DrawType::EACH_FOLLOWING_OBJECT);
			//_renderManager->createSphereSimpleForm();
			//
			//// create pipeline for merge
			//key.merge.pipeline = _renderManager->addPipeline();
			//_renderManager->configPipeline(key.merge.pipeline, gl::DrawType::NONE_OBJECT);
			//_renderManager->pushRenderPostEffectPipeline(key.merge.pipeline, key.merge.renderPostEffect);
			//_renderManager->pushRenderOnScreenPipeline(key.merge.pipeline, key.merge.renderOnScreen);
		}));
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

#ifdef PHYSIC_SIMULATION
		//addSystem<AGE::BulletDynamicSystem>(0);
		//		addSystem<CollisionAdder>(1);
		//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

		if (initRenderingJustOneTime)
		{
			initRendering();
			initRenderingJustOneTime = false;
		}

		getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/Serialized/");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("catwoman/catwoman.sage"), { MeshInfos::Positions, MeshInfos::Normals, MeshInfos::Uvs, MeshInfos::Tangents, MeshInfos::Weights, MeshInfos::BoneIndices }, "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("cube/cube.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents, MeshInfos::Weights, MeshInfos::BoneIndices }, "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("ball/ball.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents, MeshInfos::Weights, MeshInfos::BoneIndices }, "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMesh(OldFile("Sponza/sponza.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents, MeshInfos::Weights, MeshInfos::BoneIndices }, "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("cube/cube.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("ball/ball.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("catwoman/catwoman.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadMaterial(OldFile("Sponza/sponza.mage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadSkeleton(OldFile("catwoman/catwoman.skage"), "DEMO_SCENE_ASSETS");
		getInstance<AGE::AssetsManager>()->loadAnimation(OldFile("catwoman/catwoman.aage"), "DEMO_SCENE_ASSETS");

#ifdef LIFETIME_ACTIVATED
		addSystem<AGE::LifetimeSystem>(2);
#endif //!LIFETIME_ACTIVATED

		srand(42);
		return true;
	}

	bool BenchmarkScene::userUpdateBegin(double time)
	{
		++_frameCounter;
		++_chunkFrame;
		_timeCounter += time;
		_chunkCounter += time;

		std::size_t totalToLoad = 0;
		std::size_t	toLoad = 0;
		std::string loadingError;
		getInstance<AGE::AssetsManager>()->updateLoadingChannel("DEMO_SCENE_ASSETS", totalToLoad, toLoad, loadingError);
		if (loadingError.size() != 0)
			std::cout << loadingError << std::endl;
		if (toLoad != 0)
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
				cam->addPipeline(RenderType::BASIC);

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
					//_l->setPosition(glm::vec3(-5, 0, 0));
					RigidBody *rb = GLOBAL_SPONZA.addComponent<RigidBody>(0.0f);
					rb->setCollisionMesh(this, GLOBAL_SPONZA, "../../Assets/Serialized/sponza/sponza_static.phage");
					_l.setScale(glm::vec3(10.f));
				
					///////@paul first we set a mesh with only 1 submesh
					//GLOBAL_SPONZA.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage")
						//, getInstance<AGE::AssetsManager>()->getMaterial("cube/cube.mage"));
					///////@paul then we replace it with a mesh with a lot of submesh
					GLOBAL_SPONZA.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("Sponza/sponza.sage")
						, getInstance<AGE::AssetsManager>()->getMaterial("Sponza/sponza.mage"));
				}

	{
		GLOBAL_CATWOMAN = createEntity();
		auto &_l = GLOBAL_CATWOMAN.getLink();

		static bool useOnce = false;
		//auto rigidBody = GLOBAL_CATWOMAN.addComponent<RigidBody>(0);
		//rigidBody->setCollisionMesh((AScene*)(this), GLOBAL_CATWOMAN, "../../Assets/Serialized/catwoman/catwoman_dynamic.phage");
		//_l.setOrientation(glm::quat(glm::vec3(Mathematic::degreeToRadian(-90), Mathematic::degreeToRadian(90), 0)));
		_l.setPosition(glm::vec3(0, 0, 0));
		//_l.setPosition(glm::vec3(-30, 0, 0));
		//_l.setScale(glm::vec3(0.1f));
		auto _m = GLOBAL_CATWOMAN.addComponent<MeshRenderer>(
			getInstance<AGE::AssetsManager>()->getMesh("catwoman/catwoman.sage")
			, getInstance<AGE::AssetsManager>()->getMaterial("catwoman/catwoman.mage"));
	}

	for (int i = 0; i < 1; ++i)
	{
		GLOBAL_LIGHTS[i] = createEntity();
		auto e = GLOBAL_LIGHTS[i];
		auto &_l = e.getLink();
		_l.setPosition(glm::vec3(i, 1.0f, i));
		_l.setScale(glm::vec3(0.05f));
		auto _m = e.addComponent<MeshRenderer>(getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"), getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
		e.getLink().setPosition(glm::vec3(i, 5.0f, 0));
		e.addComponent<PointLightComponent>()->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.0f));
	}

	for (int i = 1; i < GLOBAL_LIGHTS.size(); ++i)
	{
		GLOBAL_LIGHTS[i] = createEntity();
		auto e = GLOBAL_LIGHTS[i];
		auto &_l = e.getLink();
		_l.setPosition(glm::vec3(i, 1.0f, i));
		_l.setScale(glm::vec3(0.05f));
		auto _m = e.addComponent<MeshRenderer>(
			getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage")
			, getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
		e.getLink().setPosition(glm::vec3(i, 5.0f, 0));
		e.addComponent<PointLightComponent>()->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.0f));
		e.getLink().attachParent(GLOBAL_LIGHTS[0].getLinkPtr());
	}



#ifdef PHYSIC_SIMULATION
	auto rigidBody = GLOBAL_FLOOR.addComponent<RigidBody>(0.0f);
	rigidBody->setCollisionShape(this, GLOBAL_FLOOR, RigidBody::BOX);
	rigidBody->getBody().setFriction(0.3f);
#endif //PHYSIC_SIMULATION


	////////////////////////////////////

	setInstance<AGE::AnimationManager>();
	auto skeleton = getInstance<AssetsManager>()->getSkeleton("catwoman/catwoman.skage");
	auto animation = getInstance<AssetsManager>()->getAnimation("catwoman/catwoman.aage");

	animationTestInstance = getInstance<AGE::AnimationManager>()->createAnimationInstance(skeleton, animation);

	auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
	
	auto i = 0;
	for (auto &e : skeleton->bones)
	{
		auto entity = createEntity();
		auto child = createEntity();
		child.addComponent<MeshRenderer>(
			getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage")
			, getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
		child.getLink().setScale(1.0f);
		entity.getLink().setPosition(posFromMat4(e.transformation));
		entity.getLink().attachChild(child.getLinkPtr());
		bonesEntities.push_back(entity);
	}

	////////////////////////////////////


	return true;
			}
		}

		auto &lc = GLOBAL_CAMERA.getLink();
		float c = 5.f;

		if (getInstance<Input>()->getPhysicalKeyPressed(AGE_LSHIFT))
			c = c * 3.0f;

		// XBOX CONTROLLER INPUTS
		Joystick controller;
		if (getInstance<Input>()->getJoystick(0, controller))
		{
			if (glm::abs(controller.axis[AGE_JOYSTICK_AXIS_LEFTY]) > 0.3)
				lc.setForward(glm::vec3(0.f, 0.f, controller.axis[AGE_JOYSTICK_AXIS_LEFTY] * c * time));
			if (glm::abs(controller.axis[AGE_JOYSTICK_AXIS_LEFTX]) > 0.3)
				lc.setForward(glm::vec3(controller.axis[AGE_JOYSTICK_AXIS_LEFTX] * c * time, 0.f, 0.f));
			if (glm::abs(controller.axis[AGE_JOYSTICK_AXIS_RIGHTX]) > 0.3)
				lc.setOrientation(glm::rotate(lc.getOrientation(), -controller.axis[AGE_JOYSTICK_AXIS_RIGHTX] * 50.f * (float)time, glm::vec3(0.f, 1.f, 0.f)));
			if (glm::abs(controller.axis[AGE_JOYSTICK_AXIS_RIGHTY]) > 0.3)
				lc.setOrientation(glm::rotate(lc.getOrientation(), -controller.axis[AGE_JOYSTICK_AXIS_RIGHTY] * 50.f * (float)time, glm::vec3(1.0f, 0.f, 0.f)));
			float leftTrigger = controller.axis[AGE_JOYSTICK_AXIS_TRIGGERLEFT] * 0.5f + 0.5f;
			float rightTrigger = controller.axis[AGE_JOYSTICK_AXIS_TRIGGERRIGHT] * 0.5f + 0.5f;
			if (leftTrigger > 0.4)
				lc.setOrientation(glm::rotate(lc.getOrientation(), leftTrigger * 50.f * (float)time, glm::vec3(0.f, 0.f, 1.f)));
			if (rightTrigger > 0.4)
				lc.setOrientation(glm::rotate(lc.getOrientation(), -rightTrigger * 50.f * (float)time, glm::vec3(0.f, 0.f, 1.f)));
		}
		else // KEYBOARD INPUT
		{
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_w))
				lc.setForward(glm::vec3(0.f, 0.f, -c * time));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_s))
				lc.setForward(glm::vec3(0.f, 0.f, c * time));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_a))
				lc.setForward(glm::vec3(-c * time, 0.f, 0.f));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_d))
				lc.setForward(glm::vec3(c * time, 0.f, 0.f));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_RIGHT))
				lc.setOrientation(glm::rotate(lc.getOrientation(), -50.f * (float)time, glm::vec3(0.f, 1.f, 0.f)));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_LEFT))
				lc.setOrientation(glm::rotate(lc.getOrientation(), 50.f * (float)time, glm::vec3(0.f, 1.f, 0.f)));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_UP))
				lc.setOrientation(glm::rotate(lc.getOrientation(), 50.f * (float)time, glm::vec3(1.f, 0.f, 0.f)));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_DOWN))
				lc.setOrientation(glm::rotate(lc.getOrientation(), -50.f * (float)time, glm::vec3(1.0f, 0.f, 0.f)));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_q))
				lc.setOrientation(glm::rotate(lc.getOrientation(), 50.f * (float)time, glm::vec3(0.f, 0.f, 1.f)));
			if (getInstance<Input>()->getPhysicalKeyPressed(AGE_e))
				lc.setOrientation(glm::rotate(lc.getOrientation(), -50.f * (float)time, glm::vec3(0.f, 0.f, 1.f)));
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
			rigidBody->setCollisionShape((AScene*)(this), e, RigidBody::BOX);
			rigidBody->getBody().setFriction(0.5f);
			rigidBody->getBody().setRestitution(0.5f);
			rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(GLOBAL_CAMERA.getLink().getOrientation() * glm::vec3(0, 0, -10)));
		}
		else
			trigger = 0.0f;

		/*if (_chunkCounter >= _maxChunk)
		{
			AScene *weakOnThis = (AScene*)(this);
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

#ifdef PHYSIC_SIMULATION
				auto rigidBody = e.addComponent<RigidBody>(1.0f);
				if (i % 4 == 0)
					rigidBody->setCollisionShape(this, e, RigidBody::SPHERE);
				else
					rigidBody->setCollisionShape(this, e, RigidBody::BOX);
				rigidBody->getBody().setFriction(0.5f);
				rigidBody->getBody().setRestitution(0.5f);
				rigidBody->getBody().applyTorque(btVector3(float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f));
#endif
			}
			_chunkCounter = 0;
		}*/

#ifdef USE_IMGUI
		if (ImGui::CollapsingHeader("Light settings"))
		{
			for (int i = 0; i < GLOBAL_LIGHTS.size(); ++i)
			{
				auto e = GLOBAL_LIGHTS[i];
				auto &link = e.getLink();

				if (ImGui::SliderFloat3(std::string("Light " + std::to_string(i) + " position").c_str(), link.getPositionPtr(), -2, 2))
				{
					auto &l = e.getLink();
					l.setPosition(l.getPosition());
				}
				auto lightComponent = e.getComponent<AGE::PointLightComponent>();
				if (ImGui::ColorEdit3(std::string("Light " + std::to_string(i) + " color").c_str(), lightComponent->getColorPtr()))
				{
					lightComponent->set(lightComponent->getColor(), lightComponent->getRange());
				}
			}
		}
#endif

		if (ImGui::Button("Reload shaders or type R") || getInstance<Input>()->getPhysicalKey(AGE_r))
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

		static float ttime = 0;
		static float timeMultiplier = 0.0f;
		ImGui::SliderFloat("Animation time", &timeMultiplier, 0.0f, 200.0f);
		getInstance<AGE::AnimationManager>()->update(timeMultiplier);
		ttime += time;
		auto &bones = getInstance<AGE::AnimationManager>()->getBones(animationTestInstance);
		auto skeleton = getInstance<AssetsManager>()->getSkeleton("catwoman/catwoman.skage");
		skeleton->updateSkinning();
		for (std::size_t i = 0; i < bones.size(); ++i)
		{
		//	bones[i] = glm::mat4(1);
			//bonesEntities[i].getLink().setPosition(posFromMat4(bones[i]));
		}
		DirtyBoneContainer::setBones(bones);
		return true;
	}


	bool BenchmarkScene::userUpdateEnd(double time)
	{
		return true;
	}
}