#ifdef SHADOWRENDERERSYSTEM_HH_

ShadowRendererSystem::ShadowRendererSystem(AScene *scene)
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
	glm::vec3 lightBase = glm::vec3(4, 8, 8);
	glm::mat4 projectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 viewMatrix = glm::lookAt(lightBase, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glDrawBuffer(GL_NONE);
	for (auto indice : _filter.getCollection())
	{
		auto &mesh = indice->getComponent<Component::ShadowRenderer>();
		mesh->render(projectionMatrix, viewMatrix);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint ShadowRendererSystem::getShadowMap() const
{
	return (_texture);
}

void ShadowRendererSystem::updateBegin(double time)
{
}

void ShadowRendererSystem::updateEnd(double time)
{
}

void ShadowRendererSystem::mainUpdate(double time)
{
}

void ShadowRendererSystem::initialize()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	_filter.require<Component::MeshRenderer>();
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, _height, _width, 0, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Somthing go wrong with the framebuffer" << std::endl;
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif /*!SHADOWRENDERERSYSTEM_HH_*/