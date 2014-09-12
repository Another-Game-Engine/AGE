#include <Systems/CameraSystem.hh>
#include <Utils/MatrixConversion.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/SceneManager.hh>
#include <Render/RenderManager.hh>
#include <Render/GeometryManager.hh>
#include <Core/Drawable.hh>
#include <Core/AssetsManager.hpp>
#include <Core/RenderThread.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <Core/PrepareRenderThreadCommand.hpp>

//tmp
#include <Core/PrepareRenderThread.hpp>

# define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Shaders/test_pipeline_1.fp"

CameraSystem::CameraSystem(std::weak_ptr<AScene> &&scene)
	: System(std::move(scene)),
#if NEW_SHADER
	_renderThread(nullptr),
	_renderManager(nullptr),
#endif
	_renderDebugMethod(false),
	_totalTime(0),
	_camera(std::move(scene)),
	_drawable(std::move(scene))
{
	_name = "camera_system";
}

void CameraSystem::setRenderDebugMode(bool t)
{
	_renderDebugMethod = t;
}

bool CameraSystem::getRenderDebugMode() const
{
	return _renderDebugMethod;
}

void CameraSystem::getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to)
{
	if (_camera.getCollection().size() == 0)
		return;
	auto scene = _scene.lock();
	auto mousePos = scene->getInstance<Input>()->getMousePosition();
	auto screenSize = scene->getInstance<AGE::Threads::Render>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
	// TODO
	//screenPosToWorldRay(mousePos.x, mousePos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform, cameraCpt->projection, from, to);
}

void CameraSystem::getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to)
{
	if (_camera.getCollection().size() == 0)
		return;
	auto scene = _scene.lock();
	auto screenSize = scene->getInstance<AGE::Threads::Render>()->getCommandQueue().safePriorityFutureEmplace<RendCtxCommand::GetScreenSize, glm::uvec2>().get();
	auto centerPos = glm::vec2(screenSize) * glm::vec2(0.5f);
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
	//TODO
	//screenPosToWorldRay(
	//	static_cast<int>(centerPos.x),
	//	static_cast<int>(centerPos.y),
	//	static_cast<int>(screenSize.x),
	//	static_cast<int>(screenSize.y),
	//	cameraCpt->lookAtTransform,
	//	cameraCpt->projection,
	//	from,
	//	to);
}

void CameraSystem::setManager()
{
	// A NETTOYER !!!!
	_renderManager = _scene.lock()->getInstance<gl::RenderManager>();
	_renderThread = (AGE::RenderThread*)(_scene.lock()->getInstance<AGE::Threads::Render>());

	assert(_renderManager != NULL && "Warning: No manager set for the camerasystem");
	
	auto res = _renderThread->getCommandQueue().safePriorityFutureEmplace<AGE::TQC::BoolFunction, bool>([&](){
		// render pass
		_shader = _renderManager->addShader(VERTEX_SHADER, FRAG_SHADER);
		_global_state = _renderManager->addUniformBlock();
		_renderManager->addShaderInterfaceBlock(_shader, "global_state", _global_state);
		_renderManager->setUniformBlock(_global_state, 1, glm::vec4(0.0f, 8.0f, 0.0f, 1.0f));
		_model_matrix = _renderManager->addShaderUniform(_shader, "model_matrix", glm::mat4(1.f));
		_view_matrix = _renderManager->addShaderUniform(_shader, "view_matrix", glm::mat4(1.f));
		_diffuse_color = _renderManager->addShaderUniform(_shader, "diffuse_color", glm::vec4(1.0f));
		_diffuse_ratio = _renderManager->addShaderUniform(_shader, "diffuse_ratio", 1.0f);
		_renderManager->bindMaterialToShader<gl::Color_diffuse>(_shader, _diffuse_color);
		_renderManager->bindMaterialToShader<gl::Ratio_diffuse>(_shader, _diffuse_ratio);
		_renderManager->bindTransformationToShader(_shader, _model_matrix);
		_renderPass = _renderManager->addRenderPass(_shader, glm::ivec4(0, 0, 800, 600));
		_renderManager->pushSetTestTaskRenderPass(_renderPass, false, false, true);
		_renderManager->pushSetClearValueTaskRenderPass(_renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
		_renderManager->pushClearTaskRenderPass(_renderPass, true, true, false);
		_renderManager->createBufferSamplableRenderPass(_renderPass, GL_COLOR_ATTACHMENT0, GL_RGB8);
		_renderManager->createBufferNotSamplableRenderPass(_renderPass, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushTargetRenderPass(_renderPass, GL_COLOR_ATTACHMENT0);
		_renderManager->pushSetBlendStateTaskRenderPass(_renderPass, 0, false);
		_renderManager->pushDrawTaskRenderBuffer(_renderPass);

		_renderOnScreen = _renderManager->addRenderOnScreen(glm::ivec4(0, 0, 800, 600));
		_renderManager->pushClearTaskRenderOnScreen(_renderOnScreen, true, true, false);
		_renderManager->pushSetTestTaskRenderOnScreen(_renderOnScreen, false, false, true);
		_renderManager->pushSetClearValueTaskRenderOnScreen(_renderOnScreen, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

		_renderManager->branch(_renderPass, _renderOnScreen);
		return true;
	});
	assert(res.get());
}

// Returns the number of seconds since the component creation
double CameraSystem::getLifeTime() const
{
	return (_totalTime);
}

void CameraSystem::updateBegin(double time)
{
}

void CameraSystem::updateEnd(double time)
{
}

void CameraSystem::mainUpdate(double time)
{
		auto renderManager = _scene.lock()->getInstance<gl::RenderManager>();
		auto renderThread = _scene.lock()->getInstance<AGE::Threads::Render>();

		auto octree = _scene.lock()->getInstance<AGE::Threads::Prepare>();
		octree->getCommandQueue().emplace<AGE::PRTC::PrepareDrawLists>([=](AGE::DrawableCollection collection)
		{
			renderManager->setUniformBlock(_global_state, 0, collection.projection);
			renderManager->setShaderUniform(_shader, _view_matrix, collection.transformation);
			renderManager->setShaderUniform(_shader, _diffuse_ratio, 1.0f);
			renderManager->draw(_renderOnScreen, _renderPass, collection.drawables);
		});
}

bool CameraSystem::initialize()
{
	_camera.requireComponent<Component::CameraComponent>();
	_drawable.requireComponent<Component::MeshRenderer>();
	return true;
}