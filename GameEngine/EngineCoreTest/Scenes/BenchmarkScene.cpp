#include <Scenes/BenchmarkScene.hpp>
#include <Configuration.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Utils/MathematicTools.hh>
#include <Skinning/AnimationManager.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Core/Commands/MainToPrepare.hpp>
#include <Core/Commands/Render.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Core/Tasks/Render.hpp>
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <Threads/TaskScheduler.hpp>

BenchmarkScene::BenchmarkScene(std::weak_ptr<AGE::Engine> engine)
	: AScene(engine)
{
}

BenchmarkScene::~BenchmarkScene(void)
{
}

#include <Render/Program.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Buffer/BufferPrograms.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
# include <Render/ProgramResources/Types/UniformBlock.hh>
# include <Render/ProgramResources/Types/Attribute.hh>
# include <Render/GeometryManagement/Painting/Painter.hh>
# include <Render/Properties/Transformation.hh>

void BenchmarkScene::initRendering()
{
	// A NETTOYER !!!!
	auto assetsManager = getInstance<AGE::AssetsManager>();
	auto mainThread = getEngine().lock();

	AGE::GetRenderThread();

	auto res = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([&]()
	{
		auto u1 = std::make_shared<UnitProg>(VERTEX_SHADER, GL_VERTEX_SHADER);
		auto u2 = std::make_shared<UnitProg>(FRAG_SHADER, GL_FRAGMENT_SHADER);
		auto program = std::make_shared<Program>(Program(std::string("basic"), { u1, u2 }));

		Painter p({ program }, { GL_FLOAT_VEC4 });
		auto key = p.add_vertices(3, 3);
		auto k = p.get_vertices(key)->add_property(std::make_shared<Transformation>(Transformation({ program })));
		auto uniformBlock = program->get_resource<UniformBlock>("global_state");
		*uniformBlock->get_resource("projection_matrix") = glm::mat4(1.0f);
		p.get_vertices(key)->set_data<glm::vec4>({ glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) }, 0);
		p.get_vertices(key)->set_indices({0, 1, 2});
		p.get_vertices(key)->get_property<Transformation>(k)->set(glm::mat4(1.0f));
		p.update();
		p.draw(GL_TRIANGLES, program, {key});
		glFlush();
		program->print_resources();
		return (true);
	});
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

#ifdef PHYSIC_SIMULATION
	setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
#endif


	std::weak_ptr<AScene> weakOnThis = std::static_pointer_cast<AScene>(shared_from_this());
//	getInstance<AGE::Threads::Prepare>()->setScene(weakOnThis); // @CESAR << remove that !!!

#ifdef PHYSIC_SIMULATION
	addSystem<BulletDynamicSystem>(0);
	//		addSystem<CollisionAdder>(1);
	//		addSystem<CollisionCleaner>(1000);
#endif //!PHYSIC

#ifdef RENDERING_ACTIVATED

	auto &camerasystem = addSystem<CameraSystem>(70); // UPDATE CAMERA AND RENDER TO SCREEN
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
	_logFile << " Rendering, ";
#ifdef PHYSIC_SIMULATION
	_logFile << " Physics, ";
#endif
	_logFile << "\n";


#endif

#ifdef RENDERING_ACTIVATED



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

		for (int i = 0; i < GLOBAL_LIGHTS.size(); ++i)
		{
			GLOBAL_LIGHTS[i] = createEntity();
			auto e = GLOBAL_LIGHTS[i];
			auto _l = getLink(e);
			_l->setPosition(glm::vec3(i, 1.0f, i));
			_l->setScale(glm::vec3(0.05f));
			auto _m = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
			// MATERIAL REFACTORING NECESSARY
			//_m->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial("ball/ball.mage"));
			//for (size_t index = 0; index < _m->getMaterial()->datas.size(); ++index)
			//{
			//	_renderManager->setMaterial<gl::Shininess>(_m->getMaterial()->datas[index], 1.0f);
			//	_renderManager->setMaterial<gl::Ratio_specular>(_m->getMaterial()->datas[index], 1.0f);
			//	_renderManager->setMaterial<gl::Color_diffuse>(_m->getMaterial()->datas[index], glm::vec4(1.0f));
			//}
			getLink(e)->setPosition(glm::vec3(i, 5.0f, 0));
			addComponent<Component::PointLight>(e)->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.0f));
		}
	

#ifdef PHYSIC_SIMULATION
	auto rigidBody = addComponent<Component::RigidBody>(GLOBAL_FLOOR, 0.0f);
	rigidBody->setCollisionShape(weakOnThis, GLOBAL_FLOOR, Component::RigidBody::BOX);
	rigidBody->getBody().setFriction(0.3f);
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

#ifdef USE_IMGUI
	//if (ImGui::Button("Save -> Clear -> Reload"))
	//{
	//	saveToBinary("SAVE_TEST.json");
	//	clearAllEntities();
	//	loadFromBinary("SAVE_TEST.json");
	//}
#endif

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

			auto link = getLink(e);
			link->setPosition(glm::vec3((rand() % 100) - 50, (rand() % 20) - 5, (rand() % 100) - 50));
			link->setOrientation(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));
			link->setScale(glm::vec3(1.0f));


			Component::MeshRenderer *mesh;
			if (i % 4 == 0)
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("ball/ball.sage"));
				// MATERIAL REFACTORING NECESSARY
				// mesh->setMaterial(getInstance<AGE::AssetsManager>()->getMaterial(File("ball/ball.mage")));
				link->setScale(glm::vec3(0.5f));
			}
			else
			{
				mesh = addComponent<Component::MeshRenderer>(e, getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"));
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
#endif

	// TODO
	AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
	// TODO
	AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::Render::RenderDrawLists>([this](AGE::DrawableCollection &collection)
	{
		// EN COURE DE CONSTRICTION :)
	});
	return true;
}