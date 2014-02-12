#ifdef SHADOWRENDERERSYSTEM_HH_

/*ShadowRendererSystem::ShadowRendererSystem(AScene *scene)
: System(scene),
_height(1000),
_width(1000),
_filter(scene)
{
	glGenTextures(1, &_texture);
	glGenFramebuffers(1, &_frameBuffer);
}

ShadowRendererSystem::~ShadowRendererSystem()
{
	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_frameBuffer);
}

void ShadowRendererSystem::render(double time)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glDrawBuffer(GL_NONE);
	for (auto indice : _filter.getCollection())
	{
		auto &mesh = indice->getComponent<Component::ShadowRenderer>();
		mesh->render(_VPLight);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint ShadowRendererSystem::getShadowMap() const
{
	return (_texture);
}*/

/*void ShadowRendererSystem::updateBegin(double time)
{
}

void ShadowRendererSystem::updateEnd(double time)
{
}

void ShadowRendererSystem::mainUpdate(double time)
{
}*/

/*void ShadowRendererSystem::initialize()
{
	
}*/

/*void ShadowRendererSystem::setLight(glm::mat4 const &VPLight)
{
	_VPLight = VPLight;
}

void ShadowRendererSystem::clearLight()
{
	_VPLight = glm::mat4(0);
}*/

#endif /*!SHADOWRENDERERSYSTEM_HH_*/