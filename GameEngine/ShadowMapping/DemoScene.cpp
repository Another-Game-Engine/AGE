#include <glm/gtc/matrix_transform.hpp>

#include "DemoScene.hh"
#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine)
: AScene(engine)
{
}

DemoScene::~DemoScene()
{
}

void DemoScene::initSytemeScene()
{
	addSystem<MeshRendererSystem>(0);
	addSystem<GraphNodeSystem>(100);
	addSystem<TrackBallSystem>(150);
	addSystem<CameraSystem>(200);
}

void DemoScene::initRenderer()
{
	std::string const perModelVars[] = { "model" };
	std::string const perFrameVars[] = { "projection", "view", "light", "time" };
	std::string const materialBasic[] = { "ambient", "diffuse", "specular", "transmittance", "emission", "shininess" };

	auto &renderer = _engine.getInstance<Renderer>();
	OpenGLTools::Shader &s = renderer.addShader("MaterialBasic", "Shaders/MaterialBasic.vp", "Shaders/MaterialBasic.fp");
	renderer.addUniform("MaterialBasic").init(&s, "MaterialBasic", materialBasic);
	renderer.addUniform("PerFrame").init(&s, "PerFrame", perFrameVars);
	renderer.addUniform("PerModel").init(&s, "PerModel", perModelVars);
	renderer.getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	renderer.bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	renderer.bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	renderer.bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");
	OpenGLTools::Shader &s2 = renderer.addShader("Basic", "Shaders/basic.vp", "Shaders/basic.fp");
	renderer.bindShaderToUniform("Basic", "PerFrame", "PerFrame");
	renderer.bindShaderToUniform("Basic", "PerModel", "PerModel");
}

void DemoScene::loadResources()
{
}

bool DemoScene::userStart()
{
	initSytemeScene();
	initRenderer();
	loadResources();
	return (true);
}

bool DemoScene::userUpdate(double time)
{
	auto input = _engine.getInstance<Input>();
	if (input.getInput(SDLK_ESCAPE) || input.getInput(SDL_QUIT))
		return (false);
	return (true);
}