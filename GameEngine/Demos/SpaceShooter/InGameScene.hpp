#ifndef		__INGAME_SCENE_HPP__
#define		__INGAME_SCENE_HPP__

#include <Core/Engine.hh>
#include <Core/AScene.hh>

/////////////
// SYSTEMS
/////////////
#include <Systems/MeshRenderSystem.h>
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
#include <Components/CameraComponent.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/VelocityComponent.hpp>
#include <Components/RigidBody.hpp>
#include <Components/SpaceshipController.hpp>

#include <MediaFiles/AssetsManager.hpp>

class InGameScene : public AScene
{
	Entity test;
public:
	InGameScene(std::weak_ptr<Engine> engine)
		: AScene(engine)
	{}

	virtual ~InGameScene(void)
	{}

	virtual bool 			userStart()
	{
		addSystem<MeshRendererSystem>(0);
		addSystem<BulletDynamicSystem>(10); // CHECK FOR COLLISIONS
		addSystem<CollisionAdder>(20); // ADD COLLISION COMPONENT TO COLLIDING ELEMENTS
		addSystem<VelocitySystem>(50); // UPDATE VELOCITY
		addSystem<SpaceshipControllerSystem>(60); // UPDATE FIRST PERSON CONTROLLER
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


		auto s = getInstance<Renderer>()->addShader("MaterialBasic",
			"./Shaders/MaterialBasic.vp",
			"./Shaders/MaterialBasic.fp");

		getInstance<Renderer>()->addUniform("MaterialBasic")
			->init(s, "MaterialBasic", materialBasic);
		getInstance<Renderer>()->addUniform("PerFrame")
			->init(s, "PerFrame", perFrameVars);
		getInstance<Renderer>()->addUniform("PerModel")
			->init(s, "PerModel", perModelVars);

		getInstance<Renderer>()->getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
		getInstance<Renderer>()->getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));
		getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
		getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
		getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

		getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__cube.cpd"));
		getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__ball.cpd"));
		getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__Space.cpd"));
		getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__galileo.cpd"));
		getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__sponza.cpd"));

		std::string		vars[] =
		{
			"projection",
			"view"
		};

		auto sky = getInstance<Renderer>()->addShader("cubemapShader", "Shaders/cubemap.vp", "Shaders/cubemap.fp");
		getInstance<Renderer>()->getShader("cubemapShader")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
		getInstance<Renderer>()->addUniform("cameraUniform")
			->init(sky, "cameraUniform", vars);
		getInstance<Renderer>()->bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");


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
			auto t = e->getLocalTransform();
			t = glm::translate(t, glm::vec3(0, 0, 0));
			t = glm::scale(t, glm::vec3(2));
			e->setLocalTransform(t);
			auto rigidBody = e->addComponent<Component::RigidBody>();
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::BOX, "obj__galileo");
			auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
			mesh->setShader("MaterialBasic");
			e->addComponent<Component::SpaceshipController>();
			heros = e;
		}

		Entity floor;
		{
			Entity e = createEntity();
			auto t = e->getLocalTransform();
			t = glm::translate(t, glm::vec3(0, -10, 0));
			t = glm::scale(t, glm::vec3(100, 100, 100));
			e->setLocalTransform(t);
			auto rigidBody = e->addComponent<Component::RigidBody>();
			rigidBody->setMass(0.0f);
			//rigidBody->setCollisionShape(Component::RigidBody::BOX);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sponza");
			auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__sponza"));
			mesh->setShader("MaterialBasic");
			floor = e;
		}


		{
			Entity e = createEntity();
			auto t = e->getLocalTransform();
			t = glm::translate(t, glm::vec3(50, -10, 50));
			t = glm::scale(t, glm::vec3(50,50,50));
			e->setLocalTransform(t);
			auto rigidBody = e->addComponent<Component::RigidBody>();
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_sponza");
			auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__sponza"));
			mesh->setShader("MaterialBasic");
			test = e;
		}

		auto camera = createEntity();
		auto cameraComponent = camera->addComponent<Component::CameraComponent>();
		auto trackBall = camera->addComponent<Component::TrackingCamera>(heros, glm::vec3(0, 0, -5.0f));
		cameraComponent->attachSkybox("skybox__space", "cubemapShader");
		return true;
	}

	Entity                  createHeros(const glm::vec3 &pos)
	{
		auto e = createEntity();
		e->setLocalTransform(glm::translate(e->getLocalTransform(), pos));
		return e;
	}

	virtual bool 			userUpdate(double time)
	{
		static std::vector<Entity> balls;
		float ftime = static_cast<float>(time);

		if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
			getInstance<Input>()->getInput(SDL_QUIT))
			return (false);
		static auto timer = 0.0f;

		if (getInstance<Input>()->getInput(SDLK_d))
		{
			for (auto &e : balls)
				destroy(e);
			balls.clear();
		}

		if (getInstance<Input>()->getInput(SDLK_u))
		{
			test->setLocalTransform(glm::scale(
				test->getLocalTransform(),
				glm::vec3(0.9f)));
		}

		if (getInstance<Input>()->getInput(SDLK_i))
		{
			test->setLocalTransform(glm::scale(test->getLocalTransform(), glm::vec3(1.1f)));
		}

		if (getInstance<Input>()->getInput(SDLK_r) && timer <= 0.0f)
		{
			timer = 0.3f;
			for (auto i = 0; i < 10; ++i)
			{
				auto e = createHeros(glm::vec3(rand() % 20, 50 + rand() % 100, rand() % 20));
				e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3((rand() % 10) / 0.8f)));
				auto rigidBody = e->addComponent<Component::RigidBody>();
				rigidBody->setMass(1.0f);
				rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_dynamic_galileo");
				auto mesh = e->addComponent<Component::MeshRenderer>(getInstance<AssetsManager>()->get<ObjFile>("obj__galileo"));
				mesh->setShader("MaterialBasic");
				balls.push_back(e);
			}
			std::cout << balls.size() << std::endl;
		}
		if (timer > 0.0f)
			timer -= ftime;		
		return (true);
	}
};

#endif //__INGAME_SCENE_HPP__