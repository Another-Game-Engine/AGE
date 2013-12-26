#ifndef		__INGAME_SCENE_HPP__
#define		__INGAME_SCENE_HPP__

#include <Core/Engine.hh>
#include <Core/AScene.hh>
#include <Entities/EntityManager.h>

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
#include <Systems/BulletSystem.hpp>
#include <Systems/VelocitySystem.hpp>

////////////
// COMPONENTS
////////////
#include <Components/CameraComponent.hh>
#include <Components/GraphNode.hpp>
#include <Components/FPController.hpp>
#include <Components/FirstPersonView.hpp>
#include <Components/VelocityComponent.hpp>

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
		addSystem<BulletSystem>(0);
		addSystem<VelocitySystem>(50);
		addSystem<GraphNodeSystem>(100);
		addSystem<TrackBallSystem>(150);
		addSystem<CameraSystem>(200);

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

		OpenGLTools::Shader &s = _engine.getInstance<Renderer>().addShader("earth",
			"./Shaders/earth.vp",
			"./Shaders/earth.fp");

		_engine.getInstance<Renderer>().addUniform("PerFrame")
			.init(&s, "PerFrame", perFrameVars);
		_engine.getInstance<Renderer>().addUniform("PerModel")
			.init(&s, "PerModel", perModelVars);

		_engine.getInstance<Renderer>().addShader("basic", "Shaders/basic.vp", "Shaders/basic.fp", "Shaders/tesselation.gp");
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
		_engine.getInstance<Renderer>().getShader("earth")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
		_engine.getInstance<Renderer>().getShader("brightnessFilter")->addTarget(GL_COLOR_ATTACHMENT1)
			.addLayer(GL_COLOR_ATTACHMENT0).build();
		_engine.getInstance<Renderer>().getShader("blurY")->addTarget(GL_COLOR_ATTACHMENT2)
			.addLayer(GL_COLOR_ATTACHMENT0).addLayer(GL_COLOR_ATTACHMENT1).build();
		_engine.getInstance<Renderer>().getUniform("PerFrame")->setUniform("light", glm::vec4(0, 0, 0, 1));
		_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("basicLight", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("basic", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("earth", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("earth", "PerModel", "PerModel");
		_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerFrame", "PerFrame");
		_engine.getInstance<Renderer>().bindShaderToUniform("bump", "PerModel", "PerModel");
		_engine.getInstance<Resources::ResourceManager>().addResource("model:ball", new Resources::SharedMesh(), "./Assets/ball.obj");
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
		_engine.getInstance<Resources::ResourceManager>().addResource("cubemap:space", new Resources::CubeMap(), "./Assets/skyboxSpace");
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

		/////////////////////////////
		/////
		/////
		/////   !!!! GAME
		/////
		////
		///
		//

		// HEROS
		auto heros = createHeros(glm::vec3(0,0,1));
		auto test = createHeros(glm::vec3(0,0,2));
		test->addComponent<Component::Velocity>(glm::vec3(0.001,0,0));

		// CAMERA
		auto camera = _engine.getInstance<EntityManager>().createEntity();
		camera->addComponent<Component::GraphNode>();
		auto cameraComponent = camera->addComponent<Component::CameraComponent>();
		auto trackBall = camera->addComponent<Component::TrackBall>(heros, 5.0f, 3.0f, 1.0f);


		cameraComponent->attachSkybox("cubemap:space", "cubemapShader");

		return true;
	}

	Handle                  createHeros(const glm::vec3 &pos)
	{
		auto e = _engine.getInstance<EntityManager>().createEntity();
		e->setLocalTransform() = glm::translate(e->getLocalTransform(), pos);
		e->addComponent<Component::GraphNode>();
		auto mesh = e->addComponent<Component::MeshRenderer>("model:ball");
		auto material = _engine.getInstance<Renderer>().getMaterialManager().createMaterial("material:heros");
		material->pushShader("basic");
		e->addComponent<Component::MaterialComponent>(std::string("material:heros"));
		auto rigidBody = e->addComponent<Component::RigidBody>(0.0f);
		rigidBody->setCollisionShape(Component::RigidBody::SPHERE);

		mesh->addTexture("texture:earth", 0);
		mesh->addTexture("texture:earthNight", 1);
		mesh->addTexture("texture:earthClouds", 2);
		mesh->addTexture("texture:earthBump", 3);
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