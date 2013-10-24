#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "BasicCam.hh"

#include "ResourceManager/SharedMesh.hh"
#include "Components/EmptyComponent.hh"
#include "Components/RotationForce.hh"

#include <SDL\SDL.h>

DemoScene::DemoScene(void)
{
}

DemoScene::~DemoScene(void)
{
}

SmartPointer<Entity> createPlanet(glm::vec3 &pos = glm::vec3(0), glm::vec3 &scale = glm::vec3(1), bool ball = true)
{
	SmartPointer<Entity>		e = new Entity;

	e->setLocalTransform() = glm::translate(glm::mat4(), pos);
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), scale);

	SmartPointer<Components::MeshRenderer>	r;
	if (ball)
		r = new Components::MeshRenderer("renderer", "model:ball");
	else
		r = new Components::MeshRenderer("renderer", "model:cube");

	GameEngine::instance()->renderer().addUniform("PerFrame")
		.registerUniform("vProjection", 0, 16 * sizeof(float))
		.registerUniform("vView", 16 * sizeof(float), 16 * sizeof(float))
		.registerUniform("fLightSpot", 2 * (16 * sizeof(float)), 3 * sizeof(float));

	GameEngine::instance()->renderer().addUniform("PerModel")
		.registerUniform("vModel", 0, 16 * sizeof(float));

	GameEngine::instance()->renderer().addShader("basicLight", "../GameEngine/Shaders/light.vp", "../GameEngine/Shaders/light.fp");
	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerModel", "PerModel");
	r->setShader("basicLight");
	e->addComponent(r);
	return e;
}

bool 			DemoScene::userStart()
{	
	GameEngine::instance()->resources().addResource("model:ball", new Resources::SharedMesh(), "../Assets/ball.obj");
	GameEngine::instance()->resources().addResource("model:cube", new Resources::SharedMesh(), "../Assets/cube.obj");

	SmartPointer<Entity> sun = createPlanet();
	SmartPointer<Components::RotationForce>	sunRot = new Components::RotationForce(glm::vec3(0, 15, 0));
	sun->addComponent(sunRot);
	getRoot()->addSon(sun);

	SmartPointer<Entity> earth = createPlanet(glm::vec3(0,0,0), glm::vec3(1), false);
	SmartPointer<Components::RotationForce>	earthRot = new Components::RotationForce(glm::vec3(0, 40, 0));
	earth->addComponent(earthRot);
	getRoot()->addSon(earth);

	setCamera(new BasicCam);

	return (true);
}

bool 			DemoScene::userUpdate()
{
	Engine		&engine = *GameEngine::instance();

	if (engine.inputs().getInput(SDLK_ESCAPE) ||
		engine.inputs().getInput(SDL_QUIT))
		return (false);
	return (true);
}
