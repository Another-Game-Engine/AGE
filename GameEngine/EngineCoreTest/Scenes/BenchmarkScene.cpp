#include <Scenes/BenchmarkScene.hpp>

#include <Configuration.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <Utils/Singleton.hh>

#include <Render/Pipeline.hh>
#include <Utils/MathematicTools.hh>
#include <Skinning/AnimationManager.hpp>
#include <Render/ResourceProgram.hh>
#include <Render/GraphicalMemory.hh>
#include <Render/Data.hh>
#include <Render/Vertices.hh>

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
	_renderManager = getInstance<RenderManager>();
	auto assetsManager = getInstance<AGE::AssetsManager>();
	_renderThread = (AGE::RenderThread*)(getInstance<AGE::Threads::Render>());

	assert(_renderManager != NULL && "Warning: No manager set for the camerasystem");

	auto res = _renderThread->getCommandQueue().safePriorityFutureEmplace<AGE::TQC::BoolFunction, bool>([&]()
	{
		GraphicalMemory graphic;

		auto &m = _renderManager;
		auto &program = m->addProgram({ m->addUnitProgram(DEFFERED_VERTEX_SHADER, GL_VERTEX_SHADER), m->addUnitProgram(DEFFERED_FRAG_SHADER, GL_FRAGMENT_SHADER) });
		auto &model_matrix = m->addResourceProgram<Mat4>(program, "model_matrix");
		m->setResourceProgram<Mat4>(program, model_matrix, glm::mat4(1.0f));
		auto key = graphic.addVertices(Vertices({ Data(std::vector<int>({1, 2, 3}), Attribute::Indices) }));
		return (true);
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
	// We register component types so that we can load components from file
	// It'll create the component manager for the scene and
	// register the type in the global component register manager
	registerComponentType<Component::CameraComponent>();
	registerComponentType<Component::MeshRenderer>();
	registerComponentType<Component::Lifetime>();
	registerComponentType<Component::RigidBody>();
	registerComponentType<Component::PointLight>();


	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
	getInstance<AGE::Threads::Prepare>()->setScene(weakOnThis);

#ifdef PHYSIC_SIMULATION
	addSystem<BulletDynamicSystem>(0);
	//		addSystem<CollisionAdder>(1);
	//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

#ifdef RENDERING_ACTIVATED

	auto &camerasystem = addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
	auto &m = *getInstance<RenderManager>();
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
	camLink->setPosition(glm::vec3(0, 1.5, 0));

	GLOBAL_FLOOR = createEntity();
	auto link = getLink(GLOBAL_FLOOR);
	link->setPosition(glm::vec3(0, -1, 0));
	link->setScale(glm::vec3(100, 1, 100));
	auto mesh = addComponent<Component::MeshRenderer>(GLOBAL_FLOOR, getInstance<AGE::AssetsManager>()->loadMesh("cube/cube.sage"));
	{
		GLOBAL_SPONZA = createEntity();
		auto _l = getLink(GLOBAL_SPONZA);
		_l->setPosition(glm::vec3(5, 0, 0));
		_l->setScale(glm::vec3(0.01f));
		auto _m = addComponent<Component::MeshRenderer>(GLOBAL_SPONZA, getInstance<AGE::AssetsManager>()->getMesh("sponza/sponza.sage"));
	}

	{
		GLOBAL_CATWOMAN = createEntity();
		auto _l = getLink(GLOBAL_CATWOMAN);

		static bool useOnce = false;
		_l->setOrientation(glm::quat(glm::vec3(Mathematic::degreeToRadian(-90), Mathematic::degreeToRadian(90), 0)));
		_l->setPosition(glm::vec3(-4, 0, 0));
		_l->setScale(glm::vec3(0.007f));
		auto _m = addComponent<Component::MeshRenderer>(GLOBAL_CATWOMAN, getInstance<AGE::AssetsManager>()->getMesh("catwoman/catwoman.sage"));
		//GLOBAL_CAT_ANIMATION = getInstance<AGE::AnimationManager>()->createAnimationInstance(
		//	getInstance<AGE::AssetsManager>()->getSkeleton("catwoman/catwoman.skage"),
		//	getInstance<AGE::AssetsManager>()->getAnimation("catwoman/catwoman-roulade.aage")
		//	);
		//_m->setAnimation(GLOBAL_CAT_ANIMATION);
	}

	{
		GLOBAL_LIGHT = createEntity();
		auto e = GLOBAL_LIGHT;
		auto _l = getLink(e);
		_l->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
		_l->setScale(glm::vec3(0.05f));
		auto _m = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
		getLink(GLOBAL_LIGHT)->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
		addComponent<Component::PointLight>(GLOBAL_LIGHT)->set(glm::vec3(1.f), glm::vec3(1.f, 0.1f, 0.0f));
	}

#ifdef PHYSIC_SIMULATION
	auto rigidBody = addComponent<Component::RigidBody>(GLOBAL_FLOOR, 0.0f);
	rigidBody->setCollisionShape(weakOnThis, GLOBAL_FLOOR, Component::RigidBody::BOX);
	rigidBody->getBody().setFriction(0.8f);
#endif //PHYSIC_SIMULATION
#endif
	// lights creation
	//addComponent<Component::PointLight>(createEntity())->set(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(1.f), glm::vec3(0.999f, 0.01f, 0.f));


	//	getLink(GLOBAL_LIGHT)->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	//addComponent<Component::PointLight>(createEntity())->set(glm::vec3(25.0f, -25.0f, 0.0f), glm::vec3(1.f), glm::vec3(0.999f, 0.01f, 0.f));
	//addComponent<Component::PointLight>(createEntity())->set(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f), glm::vec3(1.0f, 0.0f, 0.f));
	//addComponent<Component::PointLight>(createEntity())->set(glm::vec3(100.0f, 100.0f, 0.0f), glm::vec3(1.f), glm::vec3(1.0f, 0.0f, 0.f));
	//addComponent<Component::PointLight>(createEntity())->set(glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(1.f), glm::vec3(1.0f, 0.0f, 0.f));

	return true;
}

bool BenchmarkScene::userUpdate(double time)
{
	++_frameCounter;
	++_chunkFrame;
	_timeCounter += time;
	_chunkCounter += time;

	if (ImGui::Button("Save -> Clear -> Reload"))
	{
		saveToBinary("SAVE_TEST.json");
		clearAllEntities();
		loadFromBinary("SAVE_TEST.json");

		// camera creation
		//{
		//	auto camera = createEntity();
		//	GLOBAL_CAMERA = camera;
		//	auto cam = addComponent<Component::CameraComponent>(camera);

		//	auto screenSize = getInstance<AGE::RenderThread>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

		//	auto camLink = getLink(camera);
		//	camLink->setPosition(glm::vec3(0, 1.5, 0));
		//}
		// light creation
		//{
		//	GLOBAL_LIGHT = createEntity();
		//	auto e = GLOBAL_LIGHT;
		//	auto _l = getLink(e);
		//	_l->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
		//	_l->setScale(glm::vec3(0.05f));
		//	auto _m = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
		//	_m->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
		//	for (size_t index = 0; index < _m->getMaterial()->datas.size(); ++index)
		//	{
		//		_renderManager->setMaterial<gl::Shininess>(_m->getMaterial()->datas[index], 1.0f);
		//		_renderManager->setMaterial<gl::Ratio_specular>(_m->getMaterial()->datas[index], 1.0f);
		//		_renderManager->setMaterial<gl::Color_diffuse>(_m->getMaterial()->datas[index], glm::vec4(1.0f));
		//	}
		//	getLink(GLOBAL_LIGHT)->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
		//	addComponent<Component::PointLight>(GLOBAL_LIGHT)->set(glm::vec3(1.f), glm::vec3(1.f, 0.1f, 0.0f));			
		//}
	}

	//	getLink(GLOBAL_CAMERA)->setOrientation(glm::rotate(getLink(GLOBAL_CAMERA)->getOrientation(), 50.0f * (float)time, glm::vec3(0, 1, 0)));

	//if (getInstance<Input>()->getInput(SDLK_UP))
	//	getLink(GLOBAL_CAMERA)->setPosition(getLink(GLOBAL_CAMERA)->getPosition() + glm::vec3(0, 25.f * time, 0));
	//if (getInstance<Input>()->getInput(SDLK_DOWN))
	//	getLink(GLOBAL_CAMERA)->setPosition(getLink(GLOBAL_CAMERA)->getPosition() + glm::vec3(0, -25.f * time, 0));

	auto lc = getLink(GLOBAL_CAMERA);
	float c = 5.f;
	if (getInstance<Input>()->getInput(SDLK_SPACE))
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

			auto link = getLink(e);
			link->setPosition(glm::vec3((rand() % 100) - 50, (rand() % 20) - 5, (rand() % 100) - 50));
			link->setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
			link->setScale(glm::vec3(1.0f));


			Component::MeshRenderer *mesh;
			if (i % 4 == 0)
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
			}
			else
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"));
			}
#else
			auto mesh = addComponent<Component::MeshRenderer>(e, getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
			mesh->setShader("MaterialBasic");
#endif

#endif


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
	renderThread->getCommandQueue().autoEmplace<RendCtxCommand::RefreshInputs>();

	auto octree = getInstance<AGE::Threads::Prepare>();
	auto renderManager = getInstance<RenderManager>();

	{
		auto link = getLink(GLOBAL_LIGHT);
		auto pos = link->getPosition();
		static float p[3] = { pos.x, pos.y, pos.z };
		if (ImGui::SliderFloat("x", &p[0], -50, 50))
		{
			link->setPosition(glm::vec3(p[0], p[1], p[2]));
		}
		if (ImGui::SliderFloat("y", &p[1], -50, 50))
		{
			link->setPosition(glm::vec3(p[0], p[1], p[2]));
		}
		if (ImGui::SliderFloat("z", &p[2], -50, 50))
		{
			link->setPosition(glm::vec3(p[0], p[1], p[2]));
		}
	}

	//static int ooo = 0;
	//++ooo;
	//if (ooo == 100)
	//{
	//	saveToJson("SAVE_TEST.json", this);
	//	clearAllEntities();
	////	loadFromJson("SAVE_TEST.json", this);
	//	{
	//		auto camera = createEntity();
	//		GLOBAL_CAMERA = camera;
	//		auto cam = addComponent<Component::CameraComponent>(camera);

	//		auto screenSize = getInstance<AGE::RenderThread>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

	//		auto camLink = getLink(camera);
	//		camLink->setPosition(glm::vec3(0, 1.5, 0));
	//	}
	//}
	//else if (ooo == 101)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 102)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 103)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 104)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 105)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 106)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 107)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}

	//if (ooo == 600)
	//{
	//	saveToJson("SAVE_TEST.json", this);
	//	clearAllEntities();
	////	loadFromJson("SAVE_TEST.json", this);
	//	{
	//		auto camera = createEntity();
	//		GLOBAL_CAMERA = camera;
	//		auto cam = addComponent<Component::CameraComponent>(camera);

	//		auto screenSize = getInstance<AGE::RenderThread>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();

	//		auto camLink = getLink(camera);
	//		camLink->setPosition(glm::vec3(0, 1.5, 0));
	//	}
	//}
	//else if (ooo == 601)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 602)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 603)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 604)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 605)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 606)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	//else if (ooo == 607)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}


	octree->getCommandQueue().autoEmplace<AGE::PRTC::PrepareDrawLists>();

	//octree->getCommandQueue().autoEmplace<AGE::PRTC::RenderDrawLists>([=](AGE::DrawableCollection collection)
	//{
	//});

#ifdef USE_IMGUI
	ImGui::Text("Main Thread : coucou");
	ImGui::Render();
#endif

	octree->getCommandQueue().autoEmplace<AGE::PRTC::Flush>();

	octree->getCommandQueue().releaseReadability();

	return true;
}