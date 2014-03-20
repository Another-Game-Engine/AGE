
#include	<Systems/BlitFinalRender.hh>
#include	<Components/CameraComponent.hpp>

BlitFinalRender::BlitFinalRender(std::weak_ptr<AScene> scene) :
					System(scene),
					_cameraFilter(scene)
{
}

BlitFinalRender::~BlitFinalRender()
{
}

void	BlitFinalRender::initialize()
{
	_cameraFilter.requireComponent<Component::CameraComponent>();

	_quad.init(_scene);
}

void	BlitFinalRender::mainUpdate(double time)
{
	for (auto c : _cameraFilter.getCollection())
	{
		std::shared_ptr<Component::CameraComponent>		camera = c->getComponent<Component::CameraComponent>();

		OpenGLTools::Framebuffer	&current = camera->frameBuffer.isMultisampled() ? camera->downSampling : camera->frameBuffer;
		
		glViewport(camera->viewport.x, camera->viewport.y, camera->viewport.z, camera->viewport.w);

		glFinish();
		// Blit final result on back buffer:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		_quad.draw(current.getTextureAttachment(GL_COLOR_ATTACHMENT0), current.getSampleNbr(), current.getSize());
	}
}