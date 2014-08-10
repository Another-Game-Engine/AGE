#include <Systems/CameraSystem.hh>
#include <Utils/MatrixConversion.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/SceneManager.hh>
#include <Render/ShadingManager.hh>
#include <Render/GeometryManager.hh>
#include <Core/Drawable.hh>
#include <Core/AssetsManager.hpp>

//tmp
#include <Core/Octree.hpp>

# define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Shaders/test_pipeline_1.fp"

CameraSystem::CameraSystem(std::weak_ptr<AScene> &&scene)
	: System(std::move(scene)),
#if NEW_SHADER
	_render(NULL),
#endif
	_renderDebugMethod(false),
	_totalTime(0),
#if NEW_SHADER
	_camera(std::move(scene)),
	_drawable(std::move(scene))
#else
	_filter(std::move(scene))
#endif
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
#if NEW_SHADER
	if (_camera.getCollection().size() == 0)
		return;
#else
	if (_filter.getCollection().size() == 0)
		return;
#endif
	auto scene = _scene.lock();
	auto mousePos = scene->getInstance<Input>()->getMousePosition();
	auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
#if NEW_SHADER
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
#else
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_filter.getCollection().begin()));
#endif
	// TODO
	//screenPosToWorldRay(mousePos.x, mousePos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform, cameraCpt->projection, from, to);
}

void CameraSystem::getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to)
{
#if NEW_SHADER
	if (_camera.getCollection().size() == 0)
		return;
#else
	if (_filter.getCollection().size() == 0)
		return;
#endif
	auto scene = _scene.lock();
	auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
	auto centerPos = glm::vec2(screenSize) * glm::vec2(0.5f);
#if NEW_SHADER
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_camera.getCollection().begin()));
#else
	auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_filter.getCollection().begin()));
#endif
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

#if NEW_SHADER

void CameraSystem::setManager(gl::ShadingManager &m)
{
	_render = &m;

	if (_render == NULL)
		std::cerr << "Warning: No manager set for the camerasystem" << std::endl;
	
	_shader = _render->addShader(VERTEX_SHADER, FRAG_SHADER);
	size_t sizeElement[2];
	gl::set_tab_sizetype<glm::mat4, glm::vec4>(sizeElement);
	_global_state = _render->addUniformBlock(2, sizeElement);
    	_render->addShaderInterfaceBlock(_shader, "global_state", _global_state);
	_render->setUniformBlock(_global_state, 1, glm::vec4(0.0f, 8.0f, 0.0f, 1.0f));
	//_pro_matrix = _render->addShaderUniform(_shader, "projection_matrix");
	//_render->addShaderUniform(_shader, "pos_light", glm::vec4(1.0f));
	_model_matrix = _render->addShaderUniform(_shader, "model_matrix", glm::mat4(1.f));
	_view_matrix = _render->addShaderUniform(_shader, "view_matrix", glm::mat4(1.f));
	//_normal_matrix = _render->addShaderUniform(_shader, "normal_matrix", glm::mat3(1.f));
	_diffuse_texture = _render->addShaderSampler(_shader, "diffuse_texture");
	_diffuse_color = _render->addShaderUniform(_shader, "diffuse_color", glm::vec4(1.0f));
	_diffuse_ratio = _render->addShaderUniform(_shader, "diffuse_ratio", 1.0f);
	_render->bindMaterialToShader<gl::Color_diffuse>(_shader, _diffuse_color);
	_render->bindMaterialToShader<gl::Ratio_diffuse>(_shader, _diffuse_ratio);
	_render->bindTransformationToShader(_shader, _model_matrix);
	_renderPass = _render->addRender(_shader);
	_render->pushSetTestTaskRender(_renderPass, false, false, true);
	_render->pushSetClearValueTaskRender(_renderPass, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
	_render->pushClearTaskRender(_renderPass, true, true, false);
	_render->configRender(_renderPass, glm::ivec4(0, 0, 800, 600));
	//_render->addColorOutputRender(_renderPass, GL_COLOR_ATTACHMENT0, GL_RGB8);
}
#endif

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
	auto &drawList = _scene.lock()->getInstance<AGE::Octree>()->getDrawableList();
	while (!drawList.empty())
	{
		auto &camera = drawList.back();
		if (camera.drawables.empty())
			return;
		_render->setUniformBlock(_global_state, 0, camera.projection);
		_render->setShaderUniform(_shader, _view_matrix, camera.transformation);
		_render->setShaderUniform(_shader, _diffuse_ratio, 1.0f);
		//for (size_t index = 0; index < camera.drawables.size(); ++index)
		//	_render->setRenderPassMaterial(camera.drawables[index].material, _renderPass);
		_render->draw(camera.drawables);
		camera.drawables.clear();
		drawList.pop_back();
	}
}

bool CameraSystem::initialize()
{
#if !NEW_SHADER
	_filter.requireComponent<Component::CameraComponent>();
#else
	_camera.requireComponent<Component::CameraComponent>();
	_drawable.requireComponent<Component::MeshRenderer>();
#endif
	return true;
}