#ifndef		__INGAME_SCENE_HPP__
#define		__INGAME_SCENE_HPP__

#include <Core/Engine.hh>
#include <Core/AScene.hh>

/////////////
// RESOURCES
/////////////
#include <ResourceManager/Texture.hh>
#include <ResourceManager/CubeMap.hh>
#include <ResourceManager/SharedMesh.hh>

/////////////
// SYSTEMS
/////////////
#include <Systems/MeshRenderSystem.h>
#include <Systems/GraphNodeSystem.hpp>
#include <Systems/TrackBallSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/BulletCollisionSystem.hpp>
#include <Systems/VelocitySystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/SpaceshipControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>
#include <Systems/TrackingCameraSystem.hpp>

////////////
// COMPONENTS
////////////
#include <Components/CameraComponent.hh>
#include <Components/GraphNode.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/VelocityComponent.hpp>
#include <Components/RigidBody.hpp>
#include <Components/SpaceshipController.hpp>

class InGameScene : public AScene
{
public:
	InGameScene(Engine &engine)
		: AScene(engine)
	{}

	virtual ~InGameScene(void)
	{}

	virtual bool 			userStart()
	{
		addSystem<MeshRendererSystem>(0);
		addSystem<GraphNodeSystem>(1); // UPDATE GRAPH NODE POSITION
		addSystem<BulletDynamicSystem>(10); // CHECK FOR COLLISIONS
		addSystem<CollisionAdder>(20); // ADD COLLISION COMPONENT TO COLLIDING ELEMENTS
		addSystem<VelocitySystem>(50); // UPDATE VELOCITY
		addSystem<SpaceshipControllerSystem>(60); // UPDATE FIRST PERSON CONTROLLER
		addSystem<GraphNodeSystem>(70); // UPDATE GRAPH NODE POSITION
		addSystem<TrackingCameraSystem>(150); // UPDATE CAMERA TRACKING BEHAVIOR
		addSystem<FirstPersonViewSystem>(150); // UPDATE FIRST PERSON CAMERA
		addSystem<CameraSystem>(200); // UPDATE CAMERA AND RENDER TO SCREEN
		addSystem<CollisionCleaner>(300); // REMOVE COLLISION COMPONENT FROM COLLIDING ELEMENTS

		std::string		perModelVars[] =
		{
			"model"
		};

		std::string		perFrameVars[] =
		{
			"projection",
			"view",
			"light",
			"time"
		};

		std::string		materialBasic[] =
		{
			"ambient",
			"diffuse",
			"specular",
			"transmittance",
			"emission",
			"shininess"
		};


		OpenGLTools::Shader &s = _engine.getInstance<Renderer>().addShader("MaterialBasic",
			"./Shaders/MaterialBasic.vp",
			"./Shaders/MaterialBasic.fp");

		_engine.getInstance<Renderer>().addUniform("MaterialBasic")
			.init(&s, "MaterialBasic", materialBasic);
		_engine.getInstance<Renderer>().addUniform("PerFrame")
			.init(&s, "PerFrame", perFrameVars);
		_engine.getInstance<Renderer>().addUniform("PerModel")
			.init(&s, "PerModel", perModelVars);

		_engine.getInstance<Renderer>().getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
		_engine.getInstance<Renderer>().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

		SmartPointer<Resources::Texture>		toRepeat = new Resources::Texture();
		toRepeat->setWrapMode(GL_REPEAT);

		_engine.getInstance<Resources::ResourceManager>().addResource("cubemap:space", new Resources::CubeMap(), "./Assets/lake.skybox");

		std::string		vars[] =
		{
			"projection",
			"view"
		};

		OpenGLTools::Shader &sky = _engine.getInstance<Renderer>().addShader("cubemapShader", "Shaders/cubemap.vp", "Shaders/cubemap.fp");
		_engine.getInstance<Renderer>().getShader("cubemapShader")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
		_engine.getInstance<Renderer>().addUniform("cameraUniform").
			init(&sky, "cameraUniform", vars);
		_engine.getInstance<Renderer>().bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");




		////////////////////////
		/////////
		// COMPLEXE OBJ LOAD TEST
		///
		///
		_engine.getInstance<Resources::ResourceManager>().addResource("model:cube", new Resources::SharedMesh(), "./Assets/crytek-sponza/sponza.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:sponza", new Resources::SharedMesh(), "./Assets/city/city.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:cube", new Resources::SharedMesh(), "./Assets/cube/cube.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:sponza", new Resources::SharedMesh(), "./Assets/head/head.obj");
		_engine.getInstance<Resources::ResourceManager>().addResource("model:spaceship", new Resources::SharedMesh(), "./Assets/galileo/galileo.obj");
		_engine.getInstance<Resources::ResourceManager>().addResource("model:ball", new Resources::SharedMesh(), "./Assets/ball/ball.obj");

		///
		///
		///
		////////
		////////////////////////




		/////////////////////////////
		/////
		/////
		/////   !!!! GAME
		/////
		////
		///
		//

		// HEROS
		Entity heros;
		{
			Entity e = createEntity();
			e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0, 0, 0));
			e->addComponent<Component::GraphNode>();
			e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(2));
			auto rigidBody = e->addComponent<Component::RigidBody>();
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::BOX, "model:spaceship");
			auto mesh = e->addComponent<Component::MeshRenderer>("model:spaceship");
			mesh->setShader("MaterialBasic");
			e->addComponent<Component::SpaceshipController>();
			heros = e;
		}

		Entity floor;
		{
			Entity e = createEntity();
			e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0, -10, 0));
			e->addComponent<Component::GraphNode>();
			e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(100, 100, 100));
			auto rigidBody = e->addComponent<Component::RigidBody>();
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::BOX);
			auto mesh = e->addComponent<Component::MeshRenderer>("model:cube");
			mesh->setShader("MaterialBasic");
			floor = e;
		}

		auto camera = createEntity();
		auto graph = camera->addComponent<Component::GraphNode>();
		auto cameraComponent = camera->addComponent<Component::CameraComponent>();
		auto trackBall = camera->addComponent<Component::TrackingCamera>(heros, glm::vec3(0, 0, -5.0f));
		cameraComponent->attachSkybox("cubemap:space", "cubemapShader");

		return true;
	}

	Entity                  createHeros(const glm::vec3 &pos)
	{
		auto e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
		e->addComponent<Component::GraphNode>();
		return e;
	}

	virtual bool 			userUpdate(double time)
	{
		static std::vector<Entity> balls;

		if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
			_engine.getInstance<Input>().getInput(SDL_QUIT))
			return (false);
		static auto timer = 0.0f;

		if (_engine.getInstance<Input>().getInput(SDLK_d))
		{
			for (auto &e : balls)
				destroy(e);
			balls.clear();
		}

		if (_engine.getInstance<Input>().getInput(SDLK_r) && timer <= 0.0f)
		{
			timer = 0.3f;
			for (auto i = 0; i < 10; ++i)
			{
				auto e = createHeros(glm::vec3(rand() % 20, 50 + rand() % 100, rand() % 20));
				e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3((float)(rand() % 10) / 0.8));
				auto rigidBody = e->addComponent<Component::RigidBody>();
				rigidBody->setMass(1.0f);
				rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
				auto mesh = e->addComponent<Component::MeshRenderer>("model:ball");
				mesh->setShader("MaterialBasic");
				balls.push_back(e);
			}
			std::cout << balls.size() << std::endl;
		}
		if (timer > 0.0f)
			timer -= time;		
		return (true);
	}
};

#endif //__INGAME_SCENE_HPP__