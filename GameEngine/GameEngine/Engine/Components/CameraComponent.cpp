//# define GLM_FORCE_RADIANS

#include <Components/CameraComponent.hpp>
#include <glm/fwd.hpp>
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
	}

	CameraComponent::~CameraComponent(void)
	{}

	const std::string &CameraComponent::getSkyboxShader() const
	{
		return cubeMapShader;
	}
};

