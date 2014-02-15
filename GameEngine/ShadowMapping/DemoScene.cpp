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
	rct<Component::GraphNode>();
	rct<Component::MeshRenderer>();
	rct<Component::RotationForce>();
	rct<Component::TrackBall>();
	addSystem<MeshRendererSystem>(0);
	addSystem<ShadowRendererSystem>(50);
	addSystem<GraphNodeSystem>(100);
	addSystem<TrackBallSystem>(150);
	addSystem<CameraSystem>(200);
}

void DemoScene::initRenderer()
{
	std::string const lightShadow[] = {"lightMVP"};
	std::string const perModelVars[] = { "model" };
	std::string const perFrameVars[] = { "projection", "view", "light", "time" };
	std::string const materialBasic[] = { "ambient", "diffuse", "specular", "transmittance", "emission", "shininess" };

	auto &renderer = _engine.getInstance<Renderer>();
	OpenGLTools::Shader &s = renderer.addShader("MaterialBasic", "Shaders/MaterialBasic.vp", "Shaders/MaterialBasic.fp");
 	renderer.addUniform("MaterialBasic").init(&s, "MaterialBasic", materialBasic);
	renderer.addUniform("PerFrame").init(&s, "PerFrame", perFrameVars);
	renderer.addUniform("PerModel").init(&s, "PerModel", perModelVars);
	renderer.getShader("MaterialBasic")->setTextureNumber(5);
	renderer.bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	renderer.bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	renderer.bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");
	renderer.getShader("MaterialBasic")->build();

	OpenGLTools::Shader &shadow = renderer.addShader("ShadowMapping", "Shaders/ShadowMapping.vp", "Shaders/ShadowMapping.fp");
	renderer.getShader("ShadowMapping")->setTextureNumber(0);
	renderer.addUniform("Light").init(&shadow, "Light", lightShadow);
	renderer.bindShaderToUniform("ShadowMapping", "Light", "Light");
	renderer.getShader("ShadowMapping")->build();
}

void DemoScene::loadResources()
{
	AMediaFile::loadFromList("./Assets/Serialized/export__ball.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__cube.cpd");
	AMediaFile::loadFromList("./Assets/Serialized/export__sponza.cpd");
}

bool DemoScene::userStart()
{
	initSytemeScene();
	initRenderer();
	loadResources();

	getSystem<ShadowRendererSystem>()->setLight(glm::ortho<float>(-10, 10, -10, 10, 1, 20) * glm::lookAt(glm::vec3(0.5f, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	auto ball = createEntity();
	auto ballMesh = AMediaFile::get<ObjFile>("obj__ball");
	ball->addComponent<Component::GraphNode>();
	ball->setLocalTransform() = glm::translate(ball->getLocalTransform(), glm::vec3(0.0, 4.0, 4.0));
	ball->setLocalTransform() = glm::scale(ball->getLocalTransform(), glm::vec3(1.0, 1.0, 1.0));
	auto &r = ball->addComponent<Component::MeshRenderer>(ballMesh);
	r->setShader("MaterialBasic");
	auto &s = ball->addComponent<Component::ShadowRenderer>(ballMesh);
	s->setShader("ShadowMapping");

	auto platform = createEntity();
	auto platformMesh = AMediaFile::get<ObjFile>("obj__cube");
	platform->addComponent<Component::GraphNode>();
	platform->setLocalTransform() = glm::translate(platform->getLocalTransform(), glm::vec3(4.0, 0.0, 4.0));
	platform->setLocalTransform() = glm::scale(platform->getLocalTransform(), glm::vec3(8.0, 0.2, 8.0));
	auto &r2 = platform->addComponent<Component::MeshRenderer>(platformMesh);
	r2->setShader("MaterialBasic");
	auto &s2 = platform->addComponent<Component::ShadowRenderer>(platformMesh);
	s2->setShader("ShadowMapping");

	auto camera = createEntity();
	camera->addComponent<Component::GraphNode>();
	camera->addComponent<Component::CameraComponent>();
	auto trackBall = camera->addComponent<Component::TrackBall>(platform, 50.0, 3.0, 1.0);
	return (true);
}

bool DemoScene::userUpdate(double time)
{
	glClearColor(0.2, 0.2, 0.2, 1.0);
	getSystem<MeshRendererSystem>()->setTexShadow(getSystem<ShadowRendererSystem>()->getShadowMap());
	auto input = _engine.getInstance<Input>();
	if (input.getInput(SDLK_ESCAPE) || input.getInput(SDL_QUIT))
		return (false);
	return (true);
}