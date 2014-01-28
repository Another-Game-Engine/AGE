#include <glm/gtc/matrix_transform.hpp>

#include "DemoScene.hh"

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
	OpenGLTools::Shader &s = _engine.getInstance<Renderer>().addShader("MaterialBasic", "Shaders/MaterialBasic.vp", "Shaders/MaterialBasic.fp");
	renderer.addUniform("MaterialBasic").init(&s, "MaterialBasic", materialBasic);
	renderer.addUniform("PerFrame").init(&s, "PerFrame", perFrameVars);
	renderer.addUniform("PerModel").init(&s, "PerModel", perModelVars);
	renderer.getShader("MaterialBasic")->addTarget(GL_COLOR_ATTACHMENT0).setTextureNumber(4).build();
	renderer.bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	renderer.bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	renderer.bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");
}

void DemoScene::loadResources()
{
	auto &resources = _engine.getInstance<Resources::ResourceManager>();
	SmartPointer<Resources::Texture> toRepeat = new Resources::Texture();
	resources.addResource("texture:sun", new Resources::Texture(), "Assets/SunTexture.tga");
	resources.addResource("cubemap:space", new Resources::CubeMap(), "Assets/skyboxSpace");
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