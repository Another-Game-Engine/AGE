#include "DownSampleSystem.hh"

#include <Components/CameraComponent.hpp>

DownSampleSystem::DownSampleSystem(std::weak_ptr<AScene> scene) :
					System(scene),
					_cameraFilter(scene)
{
}

DownSampleSystem::~DownSampleSystem()
{
}

void	DownSampleSystem::initialize()
{
	_quad.init(_scene);
	_cameraFilter.requireComponent<Component::CameraComponent>();
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
			_quad.draw(camera->frameBuffer.getTextureAttachment(GL_COLOR_ATTACHMENT0), camera->frameBuffer.getSampleNbr(), camera->frameBuffer.getSize());
		}
	}
}
