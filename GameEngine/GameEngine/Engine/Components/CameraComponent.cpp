#include "CameraComponent.hh"
#include <Core/Engine.hh>
#include <ResourceManager/ResourceManager.hh>

namespace Component
{
	CameraComponent::CameraComponent(Engine &engine, Handle &entity)
		: ComponentBase<CameraComponent>(engine, entity, "CameraComponent")
	{}

	CameraComponent::~CameraComponent(void)
	{}

	void CameraComponent::attachSkybox(const std::string &name, const std::string &cubeMapShader)
	{
		_skybox = _engine.getInstance<Resources::ResourceManager>().getResource(name);
		_cubeMapShader = cubeMapShader;
	}

	void CameraComponent::dettachSkybox()
	{
		_skybox = nullptr;
	}

	glm::mat4         &CameraComponent::setProjection()
	{
		_entity->getFlags() & Entity::HAS_MOVED;
		return (_projection);
}

	glm::mat4 		  &CameraComponent::getProjection()
	{
		return (_projection);
	}
};

