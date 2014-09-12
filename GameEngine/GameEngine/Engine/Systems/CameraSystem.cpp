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
# define DEFFERED_VERTEX_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
# define DEFFERED_FRAG_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.fp"

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
		// create the shader
		key.shader = _renderManager->addShader(VERTEX_SHADER, FRAG_SHADER);

		// get from the shader the information key
		key.global_state = _renderManager->addUniformBlock();
		_renderManager->addShaderInterfaceBlock(key.shader, "global_state", key.global_state);
		key.view_matrix = _renderManager->addShaderUniform(key.shader, "view_matrix", glm::mat4(1.f));

		// bind the key on drawable info (material-transformation)
		_renderManager->bindMaterialToShader<gl::Color_diffuse>(key.shader, _renderManager->addShaderUniform(key.shader, "diffuse_color", glm::vec4(1.0f)));
		_renderManager->bindMaterialToShader<gl::Ratio_diffuse>(key.shader, _renderManager->addShaderUniform(key.shader, "diffuse_ratio", 1.0f));
		_renderManager->bindTransformationToShader(key.shader, _renderManager->addShaderUniform(key.shader, "model_matrix", glm::mat4(1.f)));

		// create renderpass and set it
		key.renderPass = _renderManager->addRenderPass(key.shader, glm::ivec4(0, 0, 800, 600));
		_renderManager->pushSetTestTaskRenderPass(key.renderPass, false, false, true);
		_renderManager->pushSetClearValueTaskRenderPass(key.renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
		_renderManager->pushClearTaskRenderPass(key.renderPass, true, true, false);
		_renderManager->pushTargetRenderPass(key.renderPass, GL_COLOR_ATTACHMENT0);
		_renderManager->pushTargetRenderPass(key.renderPass, GL_COLOR_ATTACHMENT1);
		_renderManager->createBufferSamplableRenderPass(key.renderPass, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.renderPass, GL_COLOR_ATTACHMENT1, GL_RGBA8);
		_renderManager->createBufferSamplableRenderPass(key.renderPass, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		_renderManager->pushSetBlendStateTaskRenderPass(key.renderPass, 0, false);
		_renderManager->pushSetBlendStateTaskRenderPass(key.renderPass, 1, false);
		_renderManager->pushDrawTaskRenderBuffer(key.renderPass);
		// create renderOnscreen and set it
		key.renderOnScreen = _renderManager->addRenderOnScreen(glm::ivec4(0, 0, 800, 600));
		_renderManager->pushClearTaskRenderOnScreen(key.renderOnScreen, true, true, false);
		_renderManager->pushSetTestTaskRenderOnScreen(key.renderOnScreen, false, false, true);
		_renderManager->pushSetClearValueTaskRenderOnScreen(key.renderOnScreen, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

		// create the pipeline and set it with both render element add before
		key.pipeline = _renderManager->addPipeline();
		_renderManager->pushRenderPassPipeline(key.pipeline, key.renderPass);
		_renderManager->pushRenderOnScreenPipeline(key.pipeline, key.renderOnScreen);
		_renderManager->branch(key.renderPass, key.renderOnScreen);
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
			renderManager->setUniformBlock(key.global_state, 0, collection.projection);
			renderManager->setShaderUniform(key.shader, key.view_matrix, collection.transformation);
			renderManager->updatePipeline(key.pipeline, collection.drawables);
			renderManager->drawPipelines();
		});
}

bool CameraSystem::initialize()
{
	_camera.requireComponent<Component::CameraComponent>();
	_drawable.requireComponent<Component::MeshRenderer>();
	return true;
}