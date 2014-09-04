#include <Scenes/BenchmarkScene.hpp>

# define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Shaders/test_pipeline_1.fp"

Entity GLOBAL_CAMERA;

BenchmarkScene::BenchmarkScene(std::weak_ptr<Engine> &&engine)
	: AScene(std::move(engine))
{
}

BenchmarkScene::~BenchmarkScene()
{
}

bool BenchmarkScene::userStart()
{
	initRenderManager();
	initLifeTime();
	initLogFramerate();
	initPhysicSimulation();
	initScene();
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
	auto &drawList = getInstance<AGE::Octree>()->getDrawableList();
	while (!drawList.empty())
	{
		auto &camera = drawList.back();
		if (camera.drawables.empty())
			return true;
		getInstance<gl::RenderManager>()->setUniformBlock(key.global_state, 0, camera.projection);
		getInstance<gl::RenderManager>()->setShaderUniform(key.shader, key.view_matrix, camera.transformation);
		getInstance<gl::RenderManager>()->updatePipeline(key.pipeline, camera.drawables);
		getInstance<gl::RenderManager>()->drawPipelines();
		camera.drawables.clear();
		drawList.pop_back();
	}
	return true;
}

BenchmarkScene &BenchmarkScene::initRenderManager()
{
	auto m = getInstance<gl::RenderManager>();

	// create the shader
	key.shader = m->addShader(VERTEX_SHADER, FRAG_SHADER); 

	// get from the shader the information key
	key.global_state = m->addUniformBlock();
	m->addShaderInterfaceBlock(key.shader, "global_state", key.global_state);
	m->setUniformBlock(key.global_state, 1, glm::vec4(0.0f, 8.0f, 0.0f, 1.0f));
	key.view_matrix = m->addShaderUniform(key.shader, "view_matrix", glm::mat4(1.f));

	// bind the key on drawable info (material-transformation)
	m->bindMaterialToShader<gl::Color_diffuse>(key.shader, m->addShaderUniform(key.shader, "diffuse_color", glm::vec4(1.0f)));
	m->bindMaterialToShader<gl::Ratio_diffuse>(key.shader, m->addShaderUniform(key.shader, "diffuse_ratio", 1.0f));
	m->bindTransformationToShader(key.shader, m->addShaderUniform(key.shader, "model_matrix", glm::mat4(1.f)));

	// create renderpass and set it
	key.renderPass = m->addRenderPass(key.shader, glm::ivec4(0, 0, 800, 600));
	m->pushSetTestTaskRenderPass(key.renderPass, false, false, true);
	m->pushSetClearValueTaskRenderPass(key.renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
	m->pushClearTaskRenderPass(key.renderPass, true, true, false);
	m->pushOutputColorRenderPass(key.renderPass, GL_COLOR_ATTACHMENT0, GL_RGB8);
	m->createDepthBufferRenderPass(key.renderPass);

	// create renderOnscreen and set it
	key.renderOnScreen = m->addRenderOnScreen(glm::ivec4(0, 0, 800, 600));
	m->pushClearTaskRenderOnScreen(key.renderOnScreen, true, true, false);
	m->pushSetTestTaskRenderOnScreen(key.renderOnScreen, false, false, true);
	m->pushSetClearValueTaskRenderOnScreen(key.renderOnScreen, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

	// create the pipeline and set it with both render element add before
	key.pipeline = m->addPipeline();
	m->setPipeline(key.pipeline, 0, key.renderPass);
	m->setPipeline(key.pipeline, 1, key.renderOnScreen);
	m->branch(key.renderPass, key.renderOnScreen);
	return (*this);
}

BenchmarkScene &BenchmarkScene::initLifeTime()
{
#ifdef LIFETIME_ACTIVATED
	addSystem<LifetimeSystem>(2);
#endif //!LIFETIME_ACTIVATED
	return (*this);
}

BenchmarkScene &BenchmarkScene::initLogFramerate()
{
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
	return (*this);
}

BenchmarkScene &BenchmarkScene::initPhysicSimulation()
{
#ifdef PHYSIC_SIMULATION
	addSystem<BulletDynamicSystem>(0);
	//		addSystem<CollisionAdder>(1);
	//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC
#ifdef RENDERING_ACTIVATED
# ifdef PHYSIC_SIMULATION
	auto rigidBody = addComponent<Component::RigidBody>(plane, 0.0f);
	rigidBody->setCollisionShape(weakOnThis, plane, Component::RigidBody::BOX);
	rigidBody->getBody().setFriction(0.8f);
# endif //PHYSIC_SIMULATION
#endif
	return (*this);
}

BenchmarkScene &BenchmarkScene::initScene()
{
	setInstance<AGE::Octree>();
	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
	getInstance<AGE::Octree>()->setScene(weakOnThis);

#ifdef RENDERING_ACTIVATED
	// build the camera
	auto camera = createEntity();
	GLOBAL_CAMERA = camera;
	auto cam = addComponent<Component::CameraComponent>(camera);

	// set the camera
	auto screenSize = getInstance<IRenderContext>()->getScreenSize();
	cam->fboSize = screenSize;
	cam->viewport = glm::uvec4(0, 0, cam->fboSize.x, cam->fboSize.y);
	cam->sampleNbr = 0;

	auto camLink = getLink(camera);
	camLink->setPosition(glm::vec3(0, 0, -10));

	// build light
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
#endif
	return (*this);
}