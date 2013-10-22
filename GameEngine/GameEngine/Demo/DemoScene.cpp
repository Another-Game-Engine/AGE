#include "Core/Engine.hh"
#include "DemoScene.hh"

#include "ResourceManager/SharedMesh.hh"

#include <SDL\SDL.h>

DemoScene::DemoScene(void)
{
}

DemoScene::~DemoScene(void)
{
}

bool 			DemoScene::userStart()
{
	SmartPointer<Entity>		e = new Entity;
	AssetsManager::instance()->addResource("model:ball", new Resources::SharedMesh(), "ball.obj");
	SmartPointer<Components::MeshRenderer>	r = new Components::MeshRenderer("renderer", "model:ball");

	OpenGLTools::UniformBuffer<>	&u = GameEngine::instance()->renderer().addUniform("PerFrame");
	
	u.init();
	u.registerUniform("vProjection", 0, 16 * sizeof(float));
	u.registerUniform("vView", 16 * sizeof(float), 16 * sizeof(float));
	u.registerUniform("fLightSpot", 2 * (16 * sizeof(float)), 3 * sizeof(float));
	u = GameEngine::instance()->renderer().addUniform("PerModel");
	u.registerUniform("vModel", 0, 16 * sizeof(float));
	u.registerUniform("fTexture", 16 * sizeof(float), 1 * sizeof(GLuint));
	GameEngine::instance()->renderer().addShader("basicLight", "shaders/light.vp", "shaders/light.fp");
	r->setShader("basicLight");
	e->addComponent(r);
	getRoot()->addSon(e);
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
