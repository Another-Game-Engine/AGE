#include <Scenes/BenchmarkScene.hpp>

#include <Configuration.hpp>
#include <Utils/Age_Imgui.hpp>

#include <Render/Pipeline.hh>
#include <Utils/MathematicTools.hh>
#include <Skinning/AnimationManager.hpp>
#include <Render/RenderManager.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Core/Commands/MainToPrepare.hpp>
#include <Core/Commands/Render.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Core/Tasks/Render.hpp>
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <Threads/TaskScheduler.hpp>

bool BenchmarkScene::initRenderingJustOneTime = true;
RenderKey BenchmarkScene::key = RenderKey();
gl::RenderManager *BenchmarkScene::_renderManager = nullptr;

BenchmarkScene::BenchmarkScene(std::weak_ptr<AGE::Engine> engine)
	: AScene(engine)
{
}

BenchmarkScene::~BenchmarkScene(void)
{
}

void BenchmarkScene::initRendering()
{
	// A NETTOYER !!!!
	_renderManager = getInstance<gl::RenderManager>();
	auto assetsManager = getInstance<AGE::AssetsManager>();
	auto mainThread = getEngine().lock();

	assert(_renderManager != NULL && "Warning: No manager set for the camerasystem");

	//TODO
	/*auto res = */AGE::GetRenderThread()->getQueue()->emplaceTask<AGE::Tasks::Basic::VoidFunction>(
		std::function<void()>([&](){
		// create the shader
		key.getBuff.shader = _renderManager->addShader(DEFFERED_VERTEX_SHADER, DEFFERED_FRAG_SHADER);
		key.Accum.shader = _renderManager->addShader(DEFFERED_VERTEX_SHADER_ACCUM, DEFFERED_FRAG_SHADER_ACCUM);
		key.merge.shader = _renderManager->addShader(DEFERRED_VERTEX_SHADER_MERGE, DEFERRED_FRAG_SHADER_MERGE);

		// get from the shader the information key
		key.global_state = _renderManager->addUniformBlock();
		_renderManager->addShaderInterfaceBlock(key.getBuff.shader, "global_state", key.global_state);
		_renderManager->addShaderInterfaceBlock(key.Accum.shader, "global_state", key.global_state);

		// bind the key on drawable info (material-transformation)
		_renderManager->bindMaterialToShader<gl::Color_specular>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "specular_color", glm::vec4(1.0f)));
		_renderManager->bindMaterialToShader<gl::Ratio_specular>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "specular_ratio", 1.0f));
		_renderManager->bindMaterialToShader<gl::Shininess>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "shininess", 0.01f));
		_renderManager->bindMaterialToShader<gl::Color_diffuse>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "diffuse_color", glm::vec4(1.0f)));
		_renderManager->bindMaterialToShader<gl::Ratio_diffuse>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "diffuse_ratio", 0.0f));
		_renderManager->bindMaterialToShader<gl::Texture_diffuse>(key.getBuff.shader, _renderManager->addShaderSampler(key.getBuff.shader, "diffuse_texture"));
		_renderManager->bindMaterialToShader<gl::Texture_normal>(key.getBuff.shader, _renderManager->addShaderSampler(key.getBuff.shader, "normal_texture"));
		_renderManager->bindTransformationToShader(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "model_matrix", glm::mat4(1.f)));

		// found uniform
		key.Accum.position_light = _renderManager->addShaderUniform(key.Accum.shader, "position_light", glm::vec3(1.f));
		key.Accum.range_light = _renderManager->addShaderUniform(key.Accum.shader, "attenuation_light", glm::vec3(1.f, 0.1f, 0.01f));
		key.Accum.color_light = _renderManager->addShaderUniform(key.Accum.shader, "color_light", glm::vec3(1.0f));
		key.Accum.depth_buffer = _renderManager->addShaderSampler(key.Accum.shader, "depth_buffer");
		key.Accum.normal_buffer = _renderManager->addShaderSampler(key.Accum.shader, "normal_buffer");
		key.Accum.specular_buffer = _renderManager->addShaderSampler(key.Accum.shader, "specular_buffer");
		key.Accum.ambiant_color = _renderManager->addShaderUniform(key.Accum.shader, "ambiant_color", glm::vec3(0.01f, 0.01f, 0.01f));
		key.merge.diffuse_buffer = _renderManager->addShaderSampler(key.merge.shader, "diffuse_buffer");
		key.merge.light_buffer = _renderManager->addShaderSampler(key.merge.shader, "light_buffer");

		// create renderpass and set it
		auto screenSize = getInstance<IRenderContext>()->getScreenSize();
		key.getBuff.renderPass = _renderManager->addRenderPass(key.getBuff.shader, glm::ivec4(0, 0, screenSize.x, screenSize.y));
		_renderManager->pushSetTestTaskRenderPass(key.getBuff.renderPass, false, false, true);
		_renderManager->pushSetClearValueTaskRenderPass(key.getBuff.renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
		_renderManager->pushClearTaskRenderPass(key.getBuff.renderPass, true, true, false);
		_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT0);
		_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT1);
		_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT2);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT1, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT2, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 0, false);
		_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 1, false);
		_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 2, false);
		_renderManager->pushSetCullFaceRenderPass(key.getBuff.renderPass, GL_BACK);
		_renderManager->pushDrawTaskRenderBuffer(key.getBuff.renderPass);

		// create  clear renderPass
		key.clean.emptyRenderPass = _renderManager->addEmptyRenderPass(glm::ivec4(0, 0, screenSize.x, screenSize.y));
		_renderManager->createBufferSamplableEmptyRenderPass(key.clean.emptyRenderPass, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		_renderManager->pushSetClearValueTaskEmptyRenderPass(key.clean.emptyRenderPass, glm::vec4(0.f, 0.f, 0.f, 1.0f));
		_renderManager->pushSetTestTaskEmptyRenderPass(key.clean.emptyRenderPass, false, false, false);
		_renderManager->pushClearTaskEmptyRenderPass(key.clean.emptyRenderPass, true, false, false);
		_renderManager->pushSetBlendStateTaskEmptyRenderPass(key.clean.emptyRenderPass, 0, false);
		_renderManager->pushTargetEmptyRenderPass(key.clean.emptyRenderPass, GL_COLOR_ATTACHMENT0);
		_renderManager->pushOwnTaskEmptyRenderPass(key.clean.emptyRenderPass, [](gl::LocationStorage &l){
			size_t nbrElement = l.getLocation<size_t>(0);
			l.setLocation(nbrElement + 1, size_t(0));
		});
		// create renderPostEffect
		gl::RenderManager *r = _renderManager;
		RenderKey *k = &key;
		key.Accum.renderPostEffect = _renderManager->addRenderPostEffect(key.Accum.shader, glm::ivec4(0, 0, screenSize.x, screenSize.y));
		_renderManager->pushSetTestTaskRenderPostEffect(key.Accum.renderPostEffect, false, false, false);
		_renderManager->pushTargetRenderPostEffect(key.Accum.renderPostEffect, GL_COLOR_ATTACHMENT0);
		_renderManager->pushSetBlendStateTaskRenderPostEffect(key.Accum.renderPostEffect, 0, true);
		_renderManager->pushSetBlendEquationTaskRenderPostEffect(key.Accum.renderPostEffect, GL_FUNC_ADD);
		_renderManager->pushSetBlendFuncTaskRenderPostEffect(key.Accum.renderPostEffect, GL_ONE, GL_ONE);
		_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.normal_buffer, GL_COLOR_ATTACHMENT1, key.getBuff.renderPass);
		_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.depth_buffer, GL_DEPTH_ATTACHMENT, key.getBuff.renderPass);
		_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.specular_buffer, GL_COLOR_ATTACHMENT2, key.getBuff.renderPass);
		_renderManager->useInputBufferRenderPostEffect(key.Accum.renderPostEffect, GL_COLOR_ATTACHMENT0, key.clean.emptyRenderPass);
		_renderManager->createBufferNotSamplableRenderPostEffect(key.Accum.renderPostEffect, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushOwnTaskRenderPostEffect(key.Accum.renderPostEffect, [=](gl::LocationStorage &l)
		{
			size_t nbrElement = l.getLocation<size_t>(0);
			size_t index = l.getLocation<size_t>(nbrElement + 1);
			AGE::PointLight pointLight = l.getLocation<AGE::PointLight>(index + 1);
			r->setShaderUniform(k->Accum.shader, k->Accum.position_light, pointLight.position);
			r->setShaderUniform(k->Accum.shader, k->Accum.range_light, pointLight.range);
			r->setShaderUniform(k->Accum.shader, k->Accum.color_light, pointLight.color);
			l.setLocation(nbrElement + 1, index + 1);
		});

		// create merge
		key.merge.renderPostEffect = _renderManager->addRenderPostEffect(key.merge.shader, glm::ivec4(0, 0, screenSize.x, screenSize.y));
		_renderManager->pushInputRenderPostEffect(key.merge.renderPostEffect, key.merge.light_buffer, GL_COLOR_ATTACHMENT0, key.Accum.renderPostEffect);
		_renderManager->pushInputRenderPostEffect(key.merge.renderPostEffect, key.merge.diffuse_buffer, GL_COLOR_ATTACHMENT0, key.getBuff.renderPass);
		_renderManager->createBufferSamplableRenderPostEffect(key.merge.renderPostEffect, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		_renderManager->createBufferNotSamplableRenderPostEffect(key.merge.renderPostEffect, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushTargetRenderPostEffect(key.merge.renderPostEffect, GL_COLOR_ATTACHMENT0);
		_renderManager->pushClearTaskRenderPostEffect(key.merge.renderPostEffect, true, false, false);
		_renderManager->pushSetTestTaskRenderPostEffect(key.merge.renderPostEffect, true, false, false);
		_renderManager->pushSetBlendStateTaskRenderPostEffect(key.merge.renderPostEffect, 0, false);

		key.merge.renderOnScreen = _renderManager->addRenderOnScreen(glm::ivec4(0, 0, screenSize.x, screenSize.y), key.merge.renderPostEffect);
		_renderManager->pushClearTaskRenderOnScreen(key.merge.renderOnScreen, true, true, false);
		_renderManager->pushSetTestTaskRenderOnScreen(key.merge.renderOnScreen, false, false, true);
		_renderManager->pushSetClearValueTaskRenderOnScreen(key.merge.renderOnScreen, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		// create the pipeline and set it with both render element add before
		key.getBuff.pipeline = _renderManager->addPipeline();
		_renderManager->pushRenderPassPipeline(key.getBuff.pipeline, key.getBuff.renderPass);

		// create the pipeline for cleaning
		key.clean.pipeline = _renderManager->addPipeline();
		_renderManager->configPipeline(key.clean.pipeline, gl::DrawType::NONE_OBJECT);
		_renderManager->pushEmptyRenderPassPipeline(key.clean.pipeline, key.clean.emptyRenderPass);

		// create the pipeline for accumulation
		key.Accum.pipeline = _renderManager->addPipeline();
		_renderManager->pushRenderPostEffectPipeline(key.Accum.pipeline, key.Accum.renderPostEffect);
		_renderManager->configPipeline(key.Accum.pipeline, gl::DrawType::EACH_FOLLOWING_OBJECT);
		_renderManager->createSphereSimpleForm();

		// create pipeline for merge
		key.merge.pipeline = _renderManager->addPipeline();
		_renderManager->configPipeline(key.merge.pipeline, gl::DrawType::NONE_OBJECT);
		_renderManager->pushRenderPostEffectPipeline(key.merge.pipeline, key.merge.renderPostEffect);
		_renderManager->pushRenderOnScreenPipeline(key.merge.pipeline, key.merge.renderOnScreen);
	}));
	//assert(res.get());
}

bool BenchmarkScene::userStart()
{
	// We register component types so that we can load components from file
	// It'll create the component manager for the scene and
	// register the type in the global component register manager
	registerComponentType<Component::CameraComponent>();
	registerComponentType<Component::MeshRenderer>();
	registerComponentType<Component::Lifetime>();
	registerComponentType<Component::RigidBody>();
	registerComponentType<Component::PointLight>();

#ifdef PHYSIC_SIMULATION
	setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
#endif

	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());

#ifdef PHYSIC_SIMULATION
	addSystem<BulletDynamicSystem>(0);
	//		addSystem<CollisionAdder>(1);
	//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

	auto &camerasystem = addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	auto &m = *getInstance<gl::RenderManager>();
	if (initRenderingJustOneTime)
	{
		initRendering();
		initRenderingJustOneTime = false;
	}

	getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/AGE-Assets-For-Test/Serialized/");
	getInstance<AGE::AssetsManager>()->loadMesh(File("cube/cube.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "DEMO_SCENE_ASSETS");
	getInstance<AGE::AssetsManager>()->loadMesh(File("ball/ball.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "DEMO_SCENE_ASSETS");
//	getInstance<AGE::AssetsManager>()->loadMesh(File("catwoman/catwoman.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "DEMO_SCENE_ASSETS");
	//getInstance<AGE::AssetsManager>()->loadMesh(File("Broken Tower/tower.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "DEMO_SCENE_ASSETS");
//	getInstance<AGE::AssetsManager>()->loadMesh(File("Venice/venice.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "DEMO_SCENE_ASSETS");
	getInstance<AGE::AssetsManager>()->loadMesh(File("Sponza/sponza.sage"), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "DEMO_SCENE_ASSETS");
	getInstance<AGE::AssetsManager>()->loadMaterial(File("cube/cube.mage"), "DEMO_SCENE_ASSETS");
	getInstance<AGE::AssetsManager>()->loadMaterial(File("ball/ball.mage"), "DEMO_SCENE_ASSETS");
//	getInstance<AGE::AssetsManager>()->loadMaterial(File("catwoman/catwoman.mage"), "DEMO_SCENE_ASSETS");
	//getInstance<AGE::AssetsManager>()->loadMaterial(File("Venice/venice.mage"), "DEMO_SCENE_ASSETS");
	//getInstance<AGE::AssetsManager>()->loadMaterial(File("Broken Tower/tower.mage"), "DEMO_SCENE_ASSETS");
	getInstance<AGE::AssetsManager>()->loadMaterial(File("Sponza/sponza.mage"), "DEMO_SCENE_ASSETS");
	//getInstance<AGE::AssetsManager>()->loadSkeleton(File("catwoman/catwoman.skage"), "DEMO_SCENE_ASSETS");
	//getInstance<AGE::AssetsManager>()->loadAnimation(File("catwoman/catwoman-roulade.aage"), "DEMO_SCENE_ASSETS");

#ifdef LIFETIME_ACTIVATED
	addSystem<LifetimeSystem>(2);
#endif //!LIFETIME_ACTIVATED

	srand(42);
	return true;
}

bool BenchmarkScene::userUpdate(double time)
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
			auto cam = addComponent<Component::CameraComponent>(camera);

			auto screeSize = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Render::GetWindowSize, glm::uvec2>().get();

			auto camLink = getLink(camera);
			camLink->setPosition(glm::vec3(0, 1.5, 0));

			GLOBAL_FLOOR = createEntity();
			auto link = getLink(GLOBAL_FLOOR);
			link->setPosition(glm::vec3(0, -0.532, 0));
			link->setScale(glm::vec3(100, 1, 100));
			auto mesh = addComponent<Component::MeshRenderer>(GLOBAL_FLOOR, getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"));
			mesh->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial("cube/cube.mage"));
			for (size_t index = 0; index < mesh->getMaterial()->datas.size(); ++index)
			{
				_renderManager->setMaterial<gl::Color_specular>(mesh->getMaterial()->datas[index], glm::vec4(1.0f));
				_renderManager->setMaterial<gl::Shininess>(mesh->getMaterial()->datas[index], 1.f);
				_renderManager->setMaterial<gl::Ratio_specular>(mesh->getMaterial()->datas[index], 1.0f);
			}
{
	GLOBAL_SPONZA = createEntity();
	auto _l = getLink(GLOBAL_SPONZA);
	//_l->setPosition(glm::vec3(-5, 0, 0));
	_l->setScale(glm::vec3(10.f));
	//_l->setOrientation(glm::quat(glm::vec3(Mathematic::degreeToRadian(-90), Mathematic::degreeToRadian(90), 0)));

	auto _m = addComponent<Component::MeshRenderer>(GLOBAL_SPONZA, getInstance<AGE::AssetsManager>()->getMesh("Sponza/sponza.sage"));
	_m->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("Sponza/sponza.mage")));
	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
//	addComponent<Component::RigidBody>(GLOBAL_SPONZA, 0.0f)->setCollisionMesh(weakOnThis, GLOBAL_SPONZA, "../../Assets/AGE-Assets-For-Test/Serialized/sponza/sponza_static.phage");
}
{
	//auto brokenTower = createEntity();
	//auto _l = getLink(brokenTower);
	//_l->setPosition(glm::vec3(0, 40, 0));
	//_l->setScale(glm::vec3(1.f));
	//_l->setOrientation(glm::quat(glm::vec3(Mathematic::degreeToRadian(-90), Mathematic::degreeToRadian(90), 0)));

	//auto _m = addComponent<Component::MeshRenderer>(brokenTower, getInstance<AGE::AssetsManager>()->getMesh("Broken Tower/tower.sage"));
	//_m->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("Broken Tower/tower.mage")));
	//auto brokenLight = createEntity();
	//_l = getLink(brokenLight);
	//_l->setPosition(glm::vec3(0, 55.0f, 0));
	//addComponent<Component::PointLight>(brokenLight)->set(glm::vec3(1.0f), glm::vec3(1.f, 0.1f, 0.0f));
}

//	{
//		GLOBAL_CATWOMAN = createEntity();
//		auto _l = getLink(GLOBAL_CATWOMAN);
//
//		static bool useOnce = false;
//		_l->setOrientation(glm::quat(glm::vec3(Mathematic::degreeToRadian(-90), Mathematic::degreeToRadian(90), 0)));
//		_l->setPosition(glm::vec3(0, 0, 0));
//		_l->setScale(glm::vec3(1.5f));
//		auto _m = addComponent<Component::MeshRenderer>(GLOBAL_CATWOMAN, getInstance<AGE::AssetsManager>()->getMesh("catwoman/catwoman.sage"));
//		_m->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("catwoman/catwoman.mage")));
//		for (size_t index = 0; index < _m->getMaterial()->datas.size(); ++index)
//		{
//			_renderManager->setMaterial<gl::Shininess>(_m->getMaterial()->datas[index], 0.1f);
//			_renderManager->setMaterial<gl::Ratio_specular>(_m->getMaterial()->datas[index], 1.0f);
//			_renderManager->setMaterial<gl::Color_specular>(_m->getMaterial()->datas[index], glm::vec4(1.0f));
//			//_renderManager->setMaterial<gl::Texture_normal>(getComponent<Component::MeshRenderer>(GLOBAL_CATWOMAN)->getMaterial()->datas[index], _renderManager->getDefaultTexture2D());
//		}
//		auto rigidBody = addComponent<Component::RigidBody>(GLOBAL_CATWOMAN, 0);
//		std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
//		rigidBody->setCollisionMesh(weakOnThis, GLOBAL_CATWOMAN, "../../Assets/AGE-Assets-For-Test/Serialized/catwoman/catwoman_static.phage");
//		rigidBody->getBody().setFriction(0.5f);
//		rigidBody->getBody().setRestitution(0.5f);
//		rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(getLink(GLOBAL_CAMERA)->getOrientation() * glm::vec3(0, 0, -10)));
//		//GLOBAL_CAT_ANIMATION = getInstance<AGE::AnimationManager>()->createAnimationInstance(
//		//	getInstance<AGE::AssetsManager>()->getSkeleton("catwoman/catwoman.skage"),
//		//	getInstance<AGE::AssetsManager>()->getAnimation("catwoman/catwoman-roulade.aage")
//		//	);
//		//_m->setAnimation(GLOBAL_CAT_ANIMATION);
//	}

	for (int i = 0; i < GLOBAL_LIGHTS.size(); ++i)
	{
		GLOBAL_LIGHTS[i] = createEntity();
		auto e = GLOBAL_LIGHTS[i];
		auto _l = getLink(e);
		_l->setPosition(glm::vec3(i, 1.0f, i));
		_l->setScale(glm::vec3(0.05f));
		auto _m = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
		_m->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
		for (size_t index = 0; index < _m->getMaterial()->datas.size(); ++index)
		{
			_renderManager->setMaterial<gl::Shininess>(_m->getMaterial()->datas[index], 1.0f);
			_renderManager->setMaterial<gl::Ratio_specular>(_m->getMaterial()->datas[index], 1.0f);
			_renderManager->setMaterial<gl::Color_diffuse>(_m->getMaterial()->datas[index], glm::vec4(1.0f));
		}
		getLink(e)->setPosition(glm::vec3(i, 5.0f, 0));
		addComponent<Component::PointLight>(e)->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.0f));
	}
	


#ifdef PHYSIC_SIMULATION
	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
	auto rigidBody = addComponent<Component::RigidBody>(GLOBAL_FLOOR, 0.0f);
	rigidBody->setCollisionShape(weakOnThis, GLOBAL_FLOOR, Component::RigidBody::BOX);
//	rigidBody->setCollisionMesh(weakOnThis, GLOBAL_FLOOR, "../../Assets/AGE-Assets-For-Test/Serialized/cube/cube_static.phage");
	rigidBody->getBody().setFriction(0.3f);
#endif //PHYSIC_SIMULATION
	return true;
		}
	}

	auto lc = getLink(GLOBAL_CAMERA);
	float c = 5.f;
	if (getInstance<Input>()->getInput(SDLK_LSHIFT))
		c = c * 3.0f;
	if (getInstance<Input>()->getInput(SDLK_z))
		lc->setForward(glm::vec3(0.f, 0.f, -c * time));
	if (getInstance<Input>()->getInput(SDLK_s))
		lc->setForward(glm::vec3(0.f, 0.f, c * time));
	if (getInstance<Input>()->getInput(SDLK_q))
		lc->setForward(glm::vec3(-c * time, 0.f, 0.f));
	if (getInstance<Input>()->getInput(SDLK_d))
		lc->setForward(glm::vec3(c * time, 0.f, 0.f));
	if (getInstance<Input>()->getInput(SDLK_RIGHT))
		lc->setOrientation(glm::rotate(lc->getOrientation(), -50.f * (float)time, glm::vec3(0.f, 1.f, 0.f)));
	if (getInstance<Input>()->getInput(SDLK_LEFT))
		lc->setOrientation(glm::rotate(lc->getOrientation(), 50.f * (float)time, glm::vec3(0.f, 1.f, 0.f)));
	if (getInstance<Input>()->getInput(SDLK_UP))
		lc->setOrientation(glm::rotate(lc->getOrientation(), 50.f * (float)time, glm::vec3(1.f, 0.f, 0.f)));
	if (getInstance<Input>()->getInput(SDLK_DOWN))
		lc->setOrientation(glm::rotate(lc->getOrientation(), -50.f * (float)time, glm::vec3(1.0f, 0.f, 0.f)));
	if (getInstance<Input>()->getInput(SDLK_a))
		lc->setOrientation(glm::rotate(lc->getOrientation(), 50.f * (float)time, glm::vec3(0.f, 0.f, 1.f)));
	if (getInstance<Input>()->getInput(SDLK_e))
		lc->setOrientation(glm::rotate(lc->getOrientation(), -50.f * (float)time, glm::vec3(0.f, 0.f, 1.f)));
	if (getInstance<Input>()->getInput(SDLK_ESCAPE))
		return (false);
	static float trigger = 0.0f;
	if (getInstance<Input>()->getInput(SDLK_SPACE) && trigger == 0.0f)
	{
		trigger += time;
		if (trigger >= 1.0f)
			trigger = 0;
		auto e = createEntity();
		addComponent<Component::Lifetime>(e, 10.0f);
		auto link = getLink(e);
		link->setPosition(getLink(GLOBAL_CAMERA)->getPosition() + glm::vec3(0, 0, -2) * getLink(GLOBAL_CAMERA)->getOrientation());
		link->setScale(glm::vec3(0.2f));
		Component::MeshRenderer *mesh;
		mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"));
		mesh->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("cube/cube.mage")));
		auto rigidBody = addComponent<Component::RigidBody>(e, 1.0f);
		std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
		rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::BOX);
		rigidBody->getBody().setFriction(0.5f);
		rigidBody->getBody().setRestitution(0.5f);
		rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(getLink(GLOBAL_CAMERA)->getOrientation() * glm::vec3(0, 0, -10)));
	}
	else
		trigger = 0.0f;

	if (_chunkCounter >= _maxChunk)
	{
		std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
		for (auto i = 0; i < 10; ++i)
		{
			auto e = createEntity();
#ifdef LIFETIME_ACTIVATED
			addComponent<Component::Lifetime>(e, 5.0f);
#endif

			auto link = getLink(e);
			link->setPosition(glm::vec3((rand() % 100) - 50, (rand() % 50) - 5, (rand() % 100) - 50));
			link->setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
			link->setScale(glm::vec3(1.0f));


			Component::MeshRenderer *mesh;
			if (i % 4 == 0)
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
				mesh->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("ball/ball.mage")));
				link->setScale(glm::vec3(0.5f));
			}
			else
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"));
				mesh->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("cube/cube.mage")));
			}

#ifdef PHYSIC_SIMULATION
			auto rigidBody = addComponent<Component::RigidBody>(e, 1.0f);
			//				rigidBody->setTransformation(link->getTransform());
			if (i % 4 == 0)
				rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::SPHERE);
			else
				rigidBody->setCollisionShape(weakOnThis, e, Component::RigidBody::BOX);
			rigidBody->getBody().setFriction(0.5f);
			rigidBody->getBody().setRestitution(0.5f);
			rigidBody->getBody().applyTorque(btVector3(float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f, float(rand() % 1000) / 300.0f));
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

	auto renderManager = getInstance<gl::RenderManager>();

#ifdef USE_IMGUI
	if (ImGui::CollapsingHeader("Light settings"))
	{
		for (int i = 0; i < GLOBAL_LIGHTS.size(); ++i)
		{
			auto e = GLOBAL_LIGHTS[i];
			auto link = getLink(e);

			if (ImGui::SliderFloat3(std::string("Light " + std::to_string(i) + " position").c_str(), link->getPositionPtr(), -50, 50))
			{
				auto l = getLink(e);
				l->setPosition(l->getPosition());
			}
			auto lightComponent = getComponent<Component::PointLight>(e);
			if (ImGui::ColorEdit3(std::string("Light " + std::to_string(i) + " color").c_str(), lightComponent->getColorPtr()))
			{
				lightComponent->set(lightComponent->getColor(), lightComponent->getRange());
			}
		}
	}
	{
		auto e = GLOBAL_SPONZA;
		auto link = getLink(e);

		if (ImGui::SliderFloat4(std::string("Sponza rotation").c_str(), link->getOrientationPtr(), -1, 1))
		{
			auto l = getLink(e);
			l->setOrientation(l->getOrientation());
		}
	}
#endif

	// TODO
	AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
	// TODO
	AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::Render::RenderDrawLists>([this, renderManager](AGE::DrawableCollection &collection)
	{
		renderManager->locationStorage.generateLocation(collection.lights.size() + 2);
		renderManager->locationStorage.setLocation(0, collection.lights.size());
		renderManager->locationStorage.setLocation(collection.lights.size() + 1, size_t(0));
		AGE::Vector<AGE::Drawable> lights;
		for (size_t index = 0; index < collection.lights.size(); ++index)
		{
			AGE::Drawable drawable;

			drawable.material = renderManager->getDefaultMaterial();
			drawable.mesh.vertices = renderManager->getSimpleFormGeo(gl::SimpleForm::SPHERE);
			drawable.mesh.indices = renderManager->getSimpleFormId(gl::SimpleForm::SPHERE);
			renderManager->locationStorage.setLocation(index + 1, collection.lights[index]);
			lights.push_back(drawable);
		}
		renderManager->setUniformBlock(key.global_state, 0, collection.projection);
		renderManager->setUniformBlock(key.global_state, 1, collection.transformation);
		renderManager->updatePipeline(key.getBuff.pipeline, collection.drawables);
		renderManager->updatePipeline(key.Accum.pipeline, lights);
		renderManager->drawPipelines();

		// Push fibonacci task to test task pool
		//		for (auto i = 0; i < 20; ++i)
		//		{
		//			AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([](){
		//				int n = rand();
		//				int fnow = 0, fnext = 1, tempf;
		//				while (--n > 0){
		//					tempf = fnow + fnext;
		//					fnow = fnext;
		//					fnext = tempf;
		//				}
		//				//std::ofstream myfile;
		//				//myfile.open(std::string(std::string("C:/Users/cesar/Desktop/trash/lol-") + std::to_string(fnext)).c_str());
		//				//myfile << AGE::Thread::threadTypeToString((AGE::Thread::ThreadType)AGE::CurrentThread()->getId());
		//				//myfile << " : " << std::to_string(fnext);
		//				//myfile << "\n";
		//				//myfile.close();
		////				std::cout << AGE::Thread::threadTypeToString((AGE::Thread::ThreadType)AGE::CurrentThread()->getId()) << " : " << fnext << std::endl;
		////				std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//			});
		//		}
	});
	return true;
}