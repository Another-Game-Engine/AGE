#include "CameraComponent.hh"
#include <Core/Engine.hh>
#include <ResourceManager/ResourceManager.hh>

namespace Component
{
	CameraComponent::CameraComponent()
		: ComponentBase<CameraComponent>(),
		projection(glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f)),
		lookAtTransform(1)
	{}

	CameraComponent::~CameraComponent(void)
	{}

	void CameraComponent::attachSkybox(const std::string &name, const std::string &_cubeMapShader)
	{
		skybox = _entity->getScene()->getEngine().getInstance<Resources::ResourceManager>().getResource(name);
		cubeMapShader = _cubeMapShader;
	}

	void CameraComponent::dettachSkybox()
	{
		skybox = nullptr;
	}

	glm::mat4         &CameraComponent::setProjection()
	{
		_entity->getFlags() & EntityData::HAS_MOVED;
		return (projection);
}

	glm::mat4 		  &CameraComponent::getProjection()
	{
		return (projection);
	}

	SmartPointer<Resources::CubeMap> CameraComponent::getSkybox()
	{
		return skybox;
	}

	const std::string &CameraComponent::getSkyboxShader() const
	{
		return cubeMapShader;
	}
};

