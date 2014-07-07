//# define GLM_FORCE_RADIANS

#include <Components/CameraComponent.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>

namespace Component
{
	CameraComponent::CameraComponent()
		: ComponentBase<CameraComponent>(),
		viewport(0),
		fboSize(800, 600),
		sampleNbr(1),
		blitOnScreen(true)
	{
			projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
			lookAtTransform = glm::mat4(1);
	}

	CameraComponent::~CameraComponent(void)
	{}

	void CameraComponent::attachSkybox(std::shared_ptr<CubeMapFile> texture, const std::string &_cubeMapShader)
	{
		skybox = texture;
		cubeMapShader = _cubeMapShader;
	}

	void CameraComponent::dettachSkybox()
	{
		skybox = nullptr;
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

