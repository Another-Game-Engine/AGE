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
#include <Systems/FPControllerSystem.hpp>
#include <Systems/FirstPersonViewSystem.hpp>

////////////
// COMPONENTS
////////////
#include <Components/CameraComponent.hh>
#include <Components/GraphNode.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/VelocityComponent.hpp>
#include <Components/RigidBody.hpp>

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
		addSystem<FPControllerSystem>(60); // UPDATE FIRST PERSON CONTROLLER
		addSystem<TrackBallSystem>(150); // UPDATE CAMERA TRACKBALL BEHAVIOR
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
		_engine.getInstance<Renderer>().addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/basic.gp");
		_engine.getInstance<Renderer>().addShader("basicLight", "Shaders/light.vp", "Shaders/light.fp");
		_engine.getInstance<Renderer>().addShader("bump", "Shaders/bump.vp", "Shaders/bump.fp");
		_engine.getInstance<Renderer>().addShader("fboToScreen", "Shaders/fboToScreen.vp", "Shaders/fboToScreen.fp");
		_engine.getInstance<Renderer>().addShader("brightnessFilter", "Shaders/brightnessFilter.vp", "Shaders/brightnessFilter.fp");
		_engine.getInstance<Renderer>().addShader("blurY", "Shaders/brightnessFilter.vp", "Shaders/blur1.fp");
		_engine.getInstance<Renderer>().getShader("basic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
		_engine.getInstance<Renderer>().getShader("basicLight")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(1).build();
		_engine.getInstance<Renderer>().getShader("bump")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(2).build();
		_engine.getInstance<Renderer>().getShader("fboToScreen")->addTarget(GL_COLOR_ATTACHMENT0)
			.addLayer(GL_COLOR_ATTACHMENT0).build();
//		_engine.getInstance<Renderer>().getShader("earth")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
		_engine.getInstance<Renderer>().getShader("brightnessFilter")->addTarget(GL_COLOR_ATTACHMENT1)
			.addLayer(GL_COLOR_ATTACHMENT0).build();
		_engine.getInstance<Renderer>().getShader("blurY")->addTarget(GL_COLOR_ATTACHMENT2)
			.addLayer(GL_COLOR_ATTACHMENT0).addLayer(GL_COLOR_ATTACHMENT1).build();
		_engine.getInstance<Renderer>().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));
		_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerModel", "PerModel");
//		_engine.getInstance<Renderer>().bindShaderToUniform("basic", "MaterialBasic", "MateriaBasic");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");
		_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerModel", "PerModel");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:ball", new Resources::SharedMesh(), "./Assets/ball.obj");
		SmartPointer<Resources::Texture>		toRepeat = new Resources::Texture();

		toRepeat->setWrapMode(GL_REPEAT);
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:sun", new Resources::Texture(), "./Assets/SunTexture.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:earth", new Resources::Texture(), "./Assets/EarthTexture.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthBump", new Resources::Texture(), "./Assets/EarthTextureBump.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthNight", new Resources::Texture(), "./Assets/EarthNightTexture.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:earthClouds", toRepeat, "./Assets/EarthClouds.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:sun", new Resources::Texture(), "./Assets/SunTexture.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:moon", new Resources::Texture(), "./Assets/MoonTexture.tga");
		_engine.getInstance<Resources::ResourceManager>().addResource("texture:moonBump", new Resources::Texture(), "./Assets/MoonNormalMap.tga");
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
   	_engine.getInstance<Resources::ResourceManager>().addResource("model:sponza", new Resources::SharedMesh(), "./Assets/dabrovic-sponza/sponza.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:sponza", new Resources::SharedMesh(), "./Assets/city/city.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:sponza", new Resources::SharedMesh(), "./Assets/cube/cube.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:ball", new Resources::SharedMesh(), "./Assets/head/head.obj");
//		_engine.getInstance<Resources::ResourceManager>().addResource("model:galileo", new Resources::SharedMesh(), "./Assets/galileo/galileo.obj");
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

		{
			auto heros = createHeros(glm::vec3(3, -10, 2));
			heros->setLocalTransform() = glm::scale(heros->getLocalTransform(), glm::vec3(100, 100, 100));
			heros->setLocalTransform() = glm::rotate(heros->getLocalTransform(), 2.0f, glm::vec3(0, 1, 1));
			auto rigidBody = heros->addComponent<Component::RigidBody>();
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::CONCAVE_STATIC_MESH, "model:sponza");
			auto mesh = heros->addComponent<Component::MeshRenderer>("model:sponza");
			mesh->setShader("MaterialBasic");
		}


		{
			auto heros = createHeros(glm::vec3(100, 100, 1));
			heros->setLocalTransform() = glm::scale(heros->getLocalTransform(), glm::vec3(1));
			heros->addComponent<Component::FPController>();
			heros->addComponent<Component::FirstPersonView>();
			auto cameraComponent = heros->addComponent<Component::CameraComponent>();
			cameraComponent->attachSkybox("cubemap:space", "cubemapShader");
		}


		{
//			auto heros = createHeros(glm::vec3(0, 100, 0));
//			heros->setLocalTransform() = glm::scale(heros->getLocalTransform(), glm::vec3(10));
////			heros->setLocalTransform() = glm::rotate(heros->getLocalTransform(), 90.0f, glm::vec3(0, 1, 1));
//			auto rigidBody = heros->addComponent<Component::RigidBody>();
//			rigidBody->setMass(1.0f);
//			rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
//			auto mesh = heros->addComponent<Component::MeshRenderer>("model:ball");
//			//heros->addComponent<Component::FPController>();
//			mesh->setShader("MaterialBasic");
//			auto camera = createEntity();
//			camera->addComponent<Component::GraphNode>();
//			auto cameraComponent = camera->addComponent<Component::CameraComponent>();
//			auto trackBall = camera->addComponent<Component::TrackBall>(heros, 5.0f, 3.0f, 1.0f);
		}



		// CAMERA
		//auto camera = createEntity();
		//camera->addComponent<Component::GraphNode>();
		//auto cameraComponent = camera->addComponent<Component::CameraComponent>();
		//auto trackBall = camera->addComponent<Component::TrackBall>(heros, 5.0f, 3.0f, 1.0f);





		return true;
	}

	Handle                  createHeros(const glm::vec3 &pos)
	{
		auto e = createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
		e->addComponent<Component::GraphNode>();
//		material->pushShader("basic");
		e->addComponent<Component::MaterialComponent>(std::string("material:heros"));
		auto material = _engine.getInstance<Renderer>().getMaterialManager().createMaterial("material:heros");
//		mesh->addTexture("texture:sun", 0);
		//mesh->addTexture("texture:earthNight", 1);
		//mesh->addTexture("texture:earthClouds", 2);
		//mesh->addTexture("texture:earthBump", 3);
		return e;
	}

	virtual bool 			userUpdate(double time)
	{
		if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
			_engine.getInstance<Input>().getInput(SDL_QUIT))
			return (false);
		return (true);
	}
};

#endif //__INGAME_SCENE_HPP__