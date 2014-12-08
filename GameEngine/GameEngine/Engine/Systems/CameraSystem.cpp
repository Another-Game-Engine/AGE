#include <Systems/CameraSystem.hh>
#include <Utils/MatrixConversion.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/SceneManager.hh>
#include <Render/RenderManager.hh>
#include <Core/PreparableObject.hh>
#include <Core/AssetsManager.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <Core/PrepareRenderThreadCommand.hpp>

//tmp
#include <Core/PrepareRenderThread.hpp>


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
	//auto screenSize = scene->getInstance<AGE::Threads::Render>()->getCommandQueue()->emplaceFuture<RendCtxCommand::GetScreenSize, glm::uvec2>().get();
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
	// TODO
	//screenPosToWorldRay(mousePos.x, mousePos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform, cameraCpt->projection, from, to);
}

void CameraSystem::getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to)
{
	if (_camera.getCollection().size() == 0)
		return;
	auto scene = _scene.lock();
	//auto screenSize = scene->getInstance<AGE::Threads::Render>()->getCommandQueue()->emplaceFuture<RendCtxCommand::GetScreenSize, glm::uvec2>().get();
	//auto centerPos = glm::vec2(screenSize) * glm::vec2(0.5f);
	//auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
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

}

bool CameraSystem::initialize()
{
	_camera.requireComponent<Component::CameraComponent>();
	_drawable.requireComponent<Component::MeshRenderer>();
	return true;
}