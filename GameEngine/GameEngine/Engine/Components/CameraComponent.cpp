#include <Components/CameraComponent.hpp>
#include <Core/Engine.hh>
#include <glm/gtc/matrix_inverse.hpp>
#include <Entities/Entity.hh>
#include <Entities/EntityData.hh>
#include <MediaFiles/AssetsManager.hpp>
#include <Core/AScene.hh>

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
		skybox = _entity->getScene()->getInstance<AssetsManager>()->get<CubeMapFile>(name);
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

	std::shared_ptr<CubeMapFile> CameraComponent::getSkybox()
	{
		return skybox;
	}

	const std::string &CameraComponent::getSkyboxShader() const
	{
		return cubeMapShader;
	}
};

