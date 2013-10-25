#include <glm/gtc/matrix_transform.hpp>

#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "BasicCam.hh"

#include "ResourceManager/SharedMesh.hh"
#include "ResourceManager/Texture.hh"
#include "Components/EmptyComponent.hh"
#include "Components/RotationForce.hh"

#include <SDL\SDL.h>

DemoScene::DemoScene(void)
{
}

DemoScene::~DemoScene(void)
{
}

SmartPointer<Entity> createPlanet(float diametre, float distance)
{
	SmartPointer<Entity>						e = new Entity;
	SmartPointer<Components::MeshRenderer>		r = new Components::MeshRenderer("renderer", "model:ball");

	r->setShader("basicLight");
	e->addComponent(r);
	e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(distance, 0, 0));
	e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(diametre));
	return (e);
}

bool 			DemoScene::userStart()
{	
	GameEngine::instance()->renderer().addUniform("PerFrame")
		.registerUniform("vProjection", 0, 16 * sizeof(float))
		.registerUniform("vView", 16 * sizeof(float), 16 * sizeof(float))
		.registerUniform("fLightSpot", 2 * (16 * sizeof(float)), 3 * sizeof(float));

	GameEngine::instance()->renderer().addUniform("PerModel")
		.registerUniform("vModel", 0, 16 * sizeof(float));

	GameEngine::instance()->renderer().addShader("basicLight", "../GameEngine/Shaders/light.vp", "../GameEngine/Shaders/light.fp");
	GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerFrame", "PerFrame");
    GameEngine::instance()->renderer().bindShaderToUniform("basicLight", "PerModel", "PerModel");

	GameEngine::instance()->resources().addResource("model:ball", new Resources::SharedMesh(), "../Assets/ball.obj");
	GameEngine::instance()->resources().addResource("test", new Resources::SharedMesh(), "../Assets/ball.obj");

	SmartPointer<Entity> sun = createPlanet(1392, 0);
	SmartPointer<Entity> planet = createPlanet(4.8, 57950);
	sun->addSon(planet);
	planet = createPlanet(12.1, 108110);
	sun->addSon(planet);
	planet = createPlanet(12.7, 149570);
	sun->addSon(planet);
	planet = createPlanet(6.7, 227840);
	sun->addSon(planet);
	planet = createPlanet(142.9, 778140);
	sun->addSon(planet);
	planet = createPlanet(116.4, 1427006);
	sun->addSon(planet);
	planet = createPlanet(46.9, 2870300);
	sun->addSon(planet);
	planet = createPlanet(43.4, 4499900);
	sun->addSon(planet);
	getRoot()->addSon(sun);
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
