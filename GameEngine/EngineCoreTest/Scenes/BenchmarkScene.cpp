#include <Scenes/BenchmarkScene.hpp>
#include <Render/Pipeline.hh>
#include <Utils/MathematicTools.hh>

BenchmarkScene::BenchmarkScene(std::weak_ptr<Engine> &&engine)
	: AScene(std::move(engine))
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
	_renderThread = (AGE::RenderThread*)(getInstance<AGE::Threads::Render>());

	assert(_renderManager != NULL && "Warning: No manager set for the camerasystem");

	auto res = _renderThread->getCommandQueue().safePriorityFutureEmplace<AGE::TQC::BoolFunction, bool>([&]()
	{
		// create the shader
		key.getBuff.shader = _renderManager->addShader(DEFFERED_VERTEX_SHADER, DEFFERED_FRAG_SHADER);
		key.Accum.shader = _renderManager->addShader(DEFFERED_VERTEX_SHADER_ACCUM, DEFFERED_FRAG_SHADER_ACCUM);
		key.merge.shader = _renderManager->addShader(DEFERRED_VERTEX_SHADER_MERGE, DEFERRED_FRAG_SHADER_MERGE);

		// get from the shader the information key
		key.global_state = _renderManager->addUniformBlock();
		_renderManager->addShaderInterfaceBlock(key.getBuff.shader, "global_state", key.global_state);
		_renderManager->addShaderInterfaceBlock(key.Accum.shader, "global_state", key.global_state);

		// bind the key on drawable info (material-transformation)
		_renderManager->bindMaterialToShader<gl::Color_diffuse>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "diffuse_color", glm::vec4(1.0f)));
		_renderManager->bindMaterialToShader<gl::Ratio_diffuse>(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "diffuse_ratio", 1.0f));
		_renderManager->bindTransformationToShader(key.getBuff.shader, _renderManager->addShaderUniform(key.getBuff.shader, "model_matrix", glm::mat4(1.f)));
		auto keyDiffuse = _renderManager->addShaderSampler(key.getBuff.shader, "diffuse_texture");
//		_renderManager->bindMaterialToShader<gl::Texture_diffuse>(key.getBuff.shader, keyDiffuse);


		// found uniform
		key.Accum.position_light = _renderManager->addShaderUniform(key.Accum.shader, "position_light", glm::vec3(1.f));
		key.Accum.range_light = _renderManager->addShaderUniform(key.Accum.shader, "attenuation_light", glm::vec3(1.f, 0.1f, 0.01f));
		key.Accum.color_light = _renderManager->addShaderUniform(key.Accum.shader, "color_light", glm::vec3(1.0f));
		key.Accum.depth_buffer = _renderManager->addShaderSampler(key.Accum.shader, "depth_buffer");
		key.Accum.normal_buffer = _renderManager->addShaderSampler(key.Accum.shader, "normal_buffer");
		key.merge.diffuse_buffer = _renderManager->addShaderSampler(key.merge.shader, "diffuse_buffer");
		key.merge.light_buffer = _renderManager->addShaderSampler(key.merge.shader, "light_buffer");

		// create renderpass and set it
		key.getBuff.renderPass = _renderManager->addRenderPass(key.getBuff.shader, glm::ivec4(0, 0, 800, 600));
		_renderManager->pushSetTestTaskRenderPass(key.getBuff.renderPass, false, false, true);
		_renderManager->pushSetClearValueTaskRenderPass(key.getBuff.renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
		_renderManager->pushClearTaskRenderPass(key.getBuff.renderPass, true, true, false);
		_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT0);
		_renderManager->pushTargetRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT1);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_COLOR_ATTACHMENT1, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.getBuff.renderPass, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 0, false);
		_renderManager->pushSetBlendStateTaskRenderPass(key.getBuff.renderPass, 1, false);
		_renderManager->pushDrawTaskRenderBuffer(key.getBuff.renderPass);

		// create  clear renderPass
		key.clean.emptyRenderPass = _renderManager->addEmptyRenderPass(glm::ivec4(0, 0, 800, 600));
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
		key.Accum.renderPostEffect = _renderManager->addRenderPostEffect(key.Accum.shader, glm::ivec4(0, 0, 800, 600));
		_renderManager->pushSetTestTaskRenderPostEffect(key.Accum.renderPostEffect, false, false, false);
		_renderManager->pushTargetRenderPostEffect(key.Accum.renderPostEffect, GL_COLOR_ATTACHMENT0);
		_renderManager->pushSetBlendStateTaskRenderPostEffect(key.Accum.renderPostEffect, 0, true);
		_renderManager->pushSetBlendEquationTaskRenderPostEffect(key.Accum.renderPostEffect, GL_FUNC_ADD);
		_renderManager->pushSetBlendFuncTaskRenderPostEffect(key.Accum.renderPostEffect, GL_ONE, GL_ONE);
		_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.normal_buffer, GL_COLOR_ATTACHMENT1, key.getBuff.renderPass);
		_renderManager->pushInputRenderPostEffect(key.Accum.renderPostEffect, key.Accum.depth_buffer, GL_DEPTH_ATTACHMENT, key.getBuff.renderPass);
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
		key.merge.renderPostEffect = _renderManager->addRenderPostEffect(key.merge.shader, glm::ivec4(0, 0, 800, 600));
		_renderManager->pushInputRenderPostEffect(key.merge.renderPostEffect, key.merge.light_buffer, GL_COLOR_ATTACHMENT0, key.Accum.renderPostEffect);
		_renderManager->pushInputRenderPostEffect(key.merge.renderPostEffect, key.merge.diffuse_buffer, GL_COLOR_ATTACHMENT0, key.getBuff.renderPass);
		_renderManager->createBufferSamplableRenderPostEffect(key.merge.renderPostEffect, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		_renderManager->createBufferNotSamplableRenderPostEffect(key.merge.renderPostEffect, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushTargetRenderPostEffect(key.merge.renderPostEffect, GL_COLOR_ATTACHMENT0);
		_renderManager->pushClearTaskRenderPostEffect(key.merge.renderPostEffect, true, false, false);
		_renderManager->pushSetTestTaskRenderPostEffect(key.merge.renderPostEffect, true, false, false);
		_renderManager->pushSetBlendStateTaskRenderPostEffect(key.merge.renderPostEffect, 0, false);

		key.merge.renderOnScreen = _renderManager->addRenderOnScreen(glm::ivec4(0, 0, 800, 600), key.merge.renderPostEffect);
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

		// create the pipeline for accum
		key.Accum.pipeline = _renderManager->addPipeline();
		_renderManager->pushRenderPostEffectPipeline(key.Accum.pipeline, key.Accum.renderPostEffect);
		_renderManager->configPipeline(key.Accum.pipeline, gl::DrawType::EACH_FOLLOWING_OBJECT);
		_renderManager->geometryManager.createSphereSimpleForm();

		// create pipleine for merge
		key.merge.pipeline = _renderManager->addPipeline();
		_renderManager->configPipeline(key.merge.pipeline, gl::DrawType::NONE_OBJECT);
		_renderManager->pushRenderPostEffectPipeline(key.merge.pipeline, key.merge.renderPostEffect);
		_renderManager->pushRenderOnScreenPipeline(key.merge.pipeline, key.merge.renderOnScreen);
		return true;
	});
	//glm::vec3 equation = glm::vec3(1-100.f, 0.1f, 0.0000001f);
	//float disc = Mathematic::secondDegreeDiscriminant(equation);
	//std::cout << disc << std::endl;
	//if (disc > 0)
	//{
	//	glm::vec2 result = Mathematic::resolveSecondDegree(equation, disc);
	//	std::cout << result.x << ", " << result.y << std::endl;
	//}
	//else
	//	std::cout << Mathematic::resolveSecondDegree(equation) << std::endl;
	assert(res.get());
}

bool BenchmarkScene::userStart()
{
	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
	getInstance<AGE::Threads::Prepare>()->setScene(weakOnThis);

#ifdef PHYSIC_SIMULATION
	addSystem<BulletDynamicSystem>(0);
	//		addSystem<CollisionAdder>(1);
	//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

#ifdef RENDERING_ACTIVATED

	auto &camerasystem = addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	auto &m = *getInstance<gl::RenderManager>();
	initRendering();

#ifdef SIMPLE_RENDERING
	addSystem<SimpleMeshRenderer>(80);
#else
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

	auto screenSize = getInstance<AGE::RenderThread>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

	auto camLink = getLink(camera);
	camLink->setPosition(glm::vec3(0, 0, -10));

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
	// lights creation
	addComponent<Component::PointLight>(createEntity())->set(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.1f, 0.f));
	addComponent<Component::PointLight>(createEntity())->set(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.1f, 0.f));
	addComponent<Component::PointLight>(createEntity())->set(glm::vec3(-5.0f, -10.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.1f, 0.f));
	addComponent<Component::PointLight>(createEntity())->set(glm::vec3(5.0f, -10.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.1f, 0.f));

	return true;
}

bool BenchmarkScene::userUpdate(double time)
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

#ifndef COMPLEX_MESH
			Component::MeshRenderer *mesh;
			if (i % 4 == 0)
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->loadMesh("ball/ball.sage"));
				mesh->setMaterial(getInstance<AGE::AssetsManager>()->loadMaterial(File("ball/ball.mage")));
				//for (size_t index = 0; index < mesh->getMaterial()->datas.size(); ++index)
				//{
				//	gl::Key<gl::Material> mat = mesh->getMaterial()->datas[index];
				//	getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Color_diffuse>(mat, glm::vec4((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, 1));
				//	getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Ratio_diffuse>(mat, 1.0f);
				//}
			}
			else
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->loadMesh("cube/cube.sage"));
				mesh->setMaterial(getInstance<AGE::AssetsManager>()->loadMaterial(File("cube/cube.mage")));
				//for (size_t index = 0; index < mesh->getMaterial()->datas.size(); ++index)
				//{
				//	gl::Key<gl::Material> mat = mesh->getMaterial()->datas[index];
				//	getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Color_diffuse>(mat, glm::vec4(0, 0.0f, 1.f, 0.f));
				//	getInstance<gl::RenderManager>()->materialManager.setMaterial<gl::Ratio_diffuse>(mat, (float)(rand() % 100) / 100.0f);
				//}
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

	auto renderThread = getInstance<AGE::RenderThread>();
	renderThread->getCommandQueue().safeEmplace<RendCtxCommand::RefreshInputs>();

	auto octree = getInstance<AGE::Threads::Prepare>();
	auto renderManager = getInstance<gl::RenderManager>();

	octree->getCommandQueue().emplace<AGE::PRTC::PrepareDrawLists>([=](AGE::DrawableCollection collection)
	{
		renderManager->locationStorage.generateLocation(collection.lights.size() + 2);
		renderManager->locationStorage.setLocation(0, collection.lights.size());
		renderManager->locationStorage.setLocation(collection.lights.size() + 1, size_t(0));
		AGE::Vector<AGE::Drawable> lights;
		for (size_t index = 0; index < collection.lights.size(); ++index)
		{
			AGE::Drawable drawable;

			drawable.material = renderManager->materialManager.getDefaultMaterial();
			drawable.mesh.vertices = renderManager->geometryManager.getSimpleFormGeo(gl::SimpleForm::SPHERE);
			drawable.mesh.indices = renderManager->geometryManager.getSimpleFormId(gl::SimpleForm::SPHERE);
			renderManager->locationStorage.setLocation(index + 1, collection.lights[index]);
			lights.push_back(drawable);
		}
		renderManager->setUniformBlock(key.global_state, 0, collection.projection);
		renderManager->setUniformBlock(key.global_state, 1, collection.transformation);
		renderManager->updatePipeline(key.getBuff.pipeline, collection.drawables);
		renderManager->updatePipeline(key.Accum.pipeline, lights);
		renderManager->drawPipelines();
	});

	octree->getCommandQueue().releaseReadability();
	return true;
}