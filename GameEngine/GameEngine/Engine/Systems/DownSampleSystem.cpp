#include "DownSampleSystem.hh"
#include <Core/AScene.hh>
#include <Components/CameraComponent.hpp>

DownSampleSystem::DownSampleSystem(std::weak_ptr<AScene> &&scene) :
					System(std::move(scene)),
					_cameraFilter(std::move(scene))
{
}

DownSampleSystem::~DownSampleSystem()
{
}

bool	DownSampleSystem::initialize()
{
	_quad.init(_scene);
	_cameraFilter.requireComponent<Component::CameraComponent>();
	return true;
}

void	DownSampleSystem::mainUpdate(double time)
{
	for (auto c : _cameraFilter.getCollection())
	{
		std::shared_ptr<Component::CameraComponent>		camera = c->getComponent<Component::CameraComponent>();

		// If multisampled, downsample the texture
		if (camera->frameBuffer.isInit() && camera->frameBuffer.isMultisampled())
		{
			// Down sample before post Fx
			camera->downSampling.bind();
			_quad.draw(camera->frameBuffer.getTextureAttachment(GL_COLOR_ATTACHMENT0)->getId(), camera->frameBuffer.getSampleNbr(), camera->frameBuffer.getSize());
		}
	}
}
