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
	rct<Component::CameraComponent>();
	rct<Component::MeshRenderer>();
	rct<Component::RotationForce>();
	rct<Component::TrackBall>();
	addSystem<MeshRendererSystem>(0)->onShadow();
	addSystem<TrackBallSystem>(150);
	addSystem<CameraSystem>(200);
}

void DemoScene::initRenderer()
{
	std::string const lightShadowBias[] = { "lightBiasMVP" };
	std::string const lightShadow[] = {"lightMVP"};
	std::string const perModelVars[] = { "model" };
	std::string const perFrameVars[] = { "projection", "view", "light", "time" };
	std::string const materialBasic[] = { "ambient", "diffuse", "specular", "transmittance", "emission", "shininess" };

	auto &renderer = _engine.getInstance<Renderer>();
	OpenGLTools::Shader &s = renderer->addShader("MaterialBasic", "Shaders/lightWithShadow.vp", "Shaders/lightWithShadow.fp");
 	renderer->addUniform("MaterialBasic").init(&s, "MaterialBasic", materialBasic);
	renderer->addUniform("PerFrame").init(&s, "PerFrame", perFrameVars);
	renderer->addUniform("PerModel").init(&s, "PerModel", perModelVars);
	renderer->addUniform("LightBias").init(&s, "LightBias", lightShadowBias);
	renderer->getShader("MaterialBasic")->setTextureNumber(5);
	renderer->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	renderer->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	renderer->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");
	renderer->bindShaderToUniform("MaterialBasic", "LightBias", "LightBias");
	renderer->getShader("MaterialBasic")->build();

	OpenGLTools::Shader &shadow = renderer->addShader("ShadowMapping", "Shaders/ShadowMapping.vp", "Shaders/ShadowMapping.fp");
	renderer->getShader("ShadowMapping")->setTextureNumber(0);
	renderer->addUniform("Light").init(&shadow, "Light", lightShadow);
	renderer->bindShaderToUniform("ShadowMapping", "Light", "Light");
	renderer->getShader("ShadowMapping")->build();
}

void DemoScene::loadResources()
{
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__ball.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__cube.cpd"));
	getInstance<AssetsManager>()->loadFromList(File("./Assets/Serialized/export__sponza.cpd"));
}

bool DemoScene::userStart()
{
	initSytemeScene();
	initRenderer();
	loadResources();

	getSystem<MeshRendererSystem>()->setLightVP(glm::ortho<float>(-10, 10, -10, 10, -10, 20) * glm::lookAt(glm::vec3(-3, 1, -1), glm::vec3(-3, 0, 0), glm::vec3(0, 1, 0)));

	auto ball = createEntity();
	auto ballMesh = getInstance<AssetsManager>()->get<ObjFile>("obj__ball");
	auto &r = ball->addComponent<Component::MeshRenderer>(ballMesh);
	r->setShader("MaterialBasic");
	ball->setLocalTransform(glm::translate(ball->getLocalTransform(), glm::vec3(0, 1.0, 0)));
	ball->setLocalTransform(glm::scale(ball->getLocalTransform(), glm::vec3(2.0, 2.0, 2.0)));

	auto platform = createEntity();
	auto platformMesh = getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
	auto &r2 = platform->addComponent<Component::MeshRenderer>(platformMesh);
	r2->setShader("MaterialBasic");
	platform->setLocalTransform(glm::translate(platform->getLocalTransform(), glm::vec3(-2.5, 0.0, -2.5)));
	platform->setLocalTransform(glm::scale(platform->getLocalTransform(), glm::vec3(10.0, 0.2, 10.0)));

	auto camera = createEntity();
	camera->addComponent<Component::CameraComponent>();
	auto trackBall = camera->addComponent<Component::TrackBall>(platform, 50.0, 3.0, 1.0);
	return (true);
}

bool DemoScene::userUpdate(double time)
{
	glClearColor(0.2, 0.2, 0.2, 1.0);
	auto input = _engine.getInstance<Input>();
	if (input->getInput(SDLK_ESCAPE) || input->getInput(SDL_QUIT))
		return (false);
	return (true);
}